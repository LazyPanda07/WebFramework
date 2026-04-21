#include "Web/Servers/ThreadPoolWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/SslException.h"
#include "Exceptions/APIException.h"
#include "HttpsNetwork.h"
#include "Utility/LargeFileHandlers/ThreadPoolHandler.h"
#include "Utility/Utils.h"
#include "Framework/WebFramework.h"

namespace framework
{
	ThreadPoolWebServer::Client::Client
	(
		SSL* ssl, SOCKET clientSocket, sockaddr address,
		std::function<void()>&& cleanup,
		const std::function<ExecutorServer::ServiceState(streams::IOSocketStream&, HttpRequestImplementation&, HttpResponseImplementation&, ResourceExecutor&, const std::function<void(ServiceState&)>&)>& service,
		ThreadPoolWebServer& server,
		DWORD timeout
	) :
		stream
		(
			server.createServerSideStream(clientSocket, ssl, std::chrono::milliseconds(timeout))
		),
		cleanup(std::move(cleanup)),
		service(service),
		address(address),
		isBusy(false),
		webExceptionAcquired(false)
	{
		web::HttpNetwork& network = stream.getNetwork<web::HttpNetwork>();

		network.setLargeBodyHandler<utility::ThreadPoolHandler>
			(
				server.additionalSettings.largeBodyPacketSize, network, server.sessionsManager, server,
				*server.resources, *server.resources,
				address, stream, *server.executorsManager, executors
			);
		network.setLargeBodySizeThreshold(server.additionalSettings.largeBodySizeThreshold);

		largeBodyHandler = &network.getLargeBodyHandler();
	}

	bool ThreadPoolWebServer::Client::serve
	(
		SessionsManager& sessionsManager,
		BaseWebServer& server,
		interfaces::IStaticFile& staticResources,
		interfaces::IDynamicFile& dynamicResources,
		ExecutorsManager& executorsManager,
		ResourceExecutor& resourceExecutor,
		threading::ThreadPool& threadPool
	)
	{
		if (stream.eof() || webExceptionAcquired)
		{
			return true;
		}

		if (isBusy || largeBodyHandler->isRunning())
		{
			return false;
		}

		if (!stream.getNetwork<web::HttpNetwork>().isDataAvailable())
		{
			return false;
		}

		HttpRequestImplementation request(sessionsManager, server, staticResources, dynamicResources, address, stream);
		HttpResponseImplementation response;
		const std::vector<std::function<void(ServiceState&)>> chain =
		{
			[this, &request](ServiceState& state)
			{
				stream >> request;

				if (stream.eof()) // request may be empty
				{
					state = ServiceState::error;
				}
				else
				{
					state = largeBodyHandler->isRunning() ? ServiceState::skipResponse : ServiceState::success;
				}
			},
			[this, &request, &response, &executorsManager, &resourceExecutor, &threadPool](ServiceState& state)
			{
				if (std::optional<std::function<void(interfaces::IHttpRequest&, interfaces::IHttpResponse&)>> threadPoolFunction = executorsManager.service(request, response, executors))
				{
					isBusy = true;

					threadPool.addTask
					(
						[this, &resourceExecutor, request = std::move(request), response = std::move(response), threadPoolFunction = std::move(threadPoolFunction)]() mutable
						{
							ServiceState state = service
							(
								stream, request, response, resourceExecutor,
								[&request, &response, &threadPoolFunction](ServiceState& _)
								{
									(*threadPoolFunction)(request, response);
								}
							);

							if (state == ServiceState::success && response)
							{
								state = service
								(
									stream, request, response, resourceExecutor,
									[this, &response, &threadPoolFunction](ServiceState& _)
									{
										stream << response;
									}
								);
							}

							if (state == ServiceState::error)
							{
								webExceptionAcquired = true;
							}
						},
						[this]() mutable
						{
							isBusy = false;
						}
					);

					state = ServiceState::skipResponse;
				}
			},
			[this, &response](ServiceState& _)
			{
				if (response)
				{
					stream << response;
				}
			}
		};
		const void* lastChainTask = &*chain.rbegin();
		const std::function<void(ServiceState&)>* task = &chain.front();

		while (task)
		{
			switch (service(stream, request, response, resourceExecutor, *task))
			{
			case framework::ExecutorServer::ServiceState::success:
				task = task == lastChainTask ? nullptr : task + 1;

				break;

			case framework::ExecutorServer::ServiceState::skipResponse:
				return false;

			case framework::ExecutorServer::ServiceState::error:
				return true;
			}
		}

		return stream.eof();
	}

	ThreadPoolWebServer::Client::~Client()
	{
		if (cleanup)
		{
			cleanup();
		}
	}

	void ThreadPoolWebServer::serveClients()
	{
		for (size_t i = 0; i < clients.size();)
		{
			Client* client = clients[i];

			bool finished = client->serve
			(
				sessionsManager,
				*this,
				*resources,
				*resources,
				*executorsManager,
				*resources,
				threadPool
			);

			if (finished)
			{
				delete client;

				clients.erase(clients.begin() + i);

				if (i)
				{
					i--;
				}
			}
			else
			{
				i++;
			}
		}
	}

	void ThreadPoolWebServer::clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr address, std::function<void()>& cleanup) //-V688
	{
		const std::optional<WebFramework::HttpsData>& httpsData = frameworkInstance.getHttpsData();
		SSL* ssl = nullptr;

		try
		{
			if (httpsData)
			{
				ssl = this->getNewSsl();

				if (!ssl)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__);
				}

				if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
				{
					throw web::exceptions::SslException(__LINE__, __FILE__);
				}

				if (int errorCode = SSL_accept(ssl); errorCode != 1)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__, ssl, errorCode);
				}
			}

			clients.push_back(new Client(ssl, clientSocket, address, std::move(cleanup), &ExecutorServer::serviceRequests, *this, timeout));
		}
		catch (const web::exceptions::SslException& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sslException, logging::category::https>(e.what(), ip);
			}

			closesocket(clientSocket);

			return;
		}

		this->serveClients();
	}

	void ThreadPoolWebServer::onInvalidConnectionReceive()
	{
		this->serveClients();
	}

	ThreadPoolWebServer::ThreadPoolWebServer
	(
		const json::JsonParser& configuration,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		std::string_view ip,
		std::string_view port,
		DWORD timeout,
		const std::vector<std::string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		uint32_t numberOfThreads,
		std::shared_ptr<threading::ThreadPool> resourcesThreadPool,
		WebFramework& frameworkInstance
	) :
		BaseTCPServer
		(
			port,
			ip,
			timeout,
			false,
			1,
			false
		),
		BaseWebServer(frameworkInstance),
		ExecutorServer
		(
			configuration,
			std::move(executorsSettings),
			pathToSources,
			additionalSettings,
			resourcesThreadPool
		),
		threadPool(numberOfThreads)
	{

	}
}
