#include "Web/Servers/ThreadPoolWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/SslException.h"
#include "Exceptions/APIException.h"
#include "Http/HttpsNetwork.h"
#include "Utility/LargeFileHandlers/ThreadPoolHandler.h"
#include "Utility/Utils.h"
#include "Framework/WebFramework.h"

namespace framework
{
	ThreadPoolWebServer::Client::Client
	(
		SSL* ssl, SOCKET clientSocket, sockaddr address,
		std::function<void()>&& cleanup,
		const serve_loop::HttpServeLoop::HttpServeTaskSignature& serveTask,
		ThreadPoolWebServer& server,
		DWORD timeout
	) :
		cleanup(std::move(cleanup)),
		isBusy(false),
		webExceptionAcquired(false)
	{
		auto serveRequest = [this, &server](HttpRequestImplementation& request, HttpResponseImplementation& response, ExecutorsManager::StatefulExecutors& executors, std::queue<std::unique_ptr<event::ServeEvent>>& events, ServiceState& state)
			{
				std::optional<std::function<void(interfaces::IHttpRequest&, interfaces::IHttpResponse&)>> threadPoolFunction = server.executorsManager->service(request, response, executors, events);

				if (threadPoolFunction)
				{
					isBusy = true;

					server.threadPool.addTask
					(
						[this, &server, &request, &response, threadPoolFunction = std::move(threadPoolFunction)]() mutable
						{
							streams::IOSocketStream& stream = loop->getStream();

							ServiceState state = ExecutorServer::serveTask
							(
								stream, request, response, *server.resources,
								[&request, &response, &threadPoolFunction](ServiceState& _)
								{
									(*threadPoolFunction)(request, response);
								}
							);

							if (state == ServiceState::success && response)
							{
								state = ExecutorServer::serveTask
								(
									stream, request, response, *server.resources,
									[this, &stream, &response, &threadPoolFunction](ServiceState& _)
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
			};

		loop = std::make_unique<serve_loop::HttpServeLoop>
			(
				server.createServerSideStream(clientSocket, ssl, std::chrono::milliseconds(timeout)),
				*server.getResourceExecutor(),
				serveTask,
				serveRequest,
				server.sessionsManager,
				server,
				*server.executorsManager,
				address,
				server.additionalSettings,
				events
			);
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
		streams::IOSocketStream& stream = loop->getStream();
		const web::http::HttpNetwork& network = stream.getNetwork<web::http::HttpNetwork>();
		
		if (stream.eof() || webExceptionAcquired)
		{
			return true;
		}

		if (isBusy || network.getLargeBodyHandler().isRunning())
		{
			return false;
		}

		if (!network.isDataAvailable())
		{
			return false;
		}

		return serve_loop::ServeLoop::runLoop(loop, events) || stream.eof();
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

				if (int errorCode = SSL_set_fd(ssl, static_cast<int>(clientSocket)); errorCode != 1)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__, ssl, errorCode);
				}

				if (int errorCode = SSL_accept(ssl); errorCode != 1)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__, ssl, errorCode);
				}
			}

			clients.push_back
			(
				new Client
				(
					ssl,
					clientSocket,
					address,
					std::move(cleanup),
					&ExecutorServer::serveTask,
					*this, 
					timeout
				)
			);
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
