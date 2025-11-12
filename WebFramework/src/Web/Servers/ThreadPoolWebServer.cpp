#include "ThreadPoolWebServer.h"

#include <Log.h>

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/NotFoundException.h"
#include "Exceptions/SslException.h"
#include "Exceptions/APIException.h"
#include "Utility/Singletons/HTTPSSingleton.h"
#include "HttpsNetwork.h"
#include "Utility/LargeFileHandlers/ThreadPoolHandler.h"

namespace framework
{
	ThreadPoolWebServer::Client::Client
	(
		SSL* ssl, SSL_CTX* context, SOCKET clientSocket, sockaddr address,
		std::function<void()>&& cleanup,
		const std::function<ExecutorServer::ServiceState(streams::IOSocketStream&, HTTPRequestImplementation&, HTTPResponseImplementation&, ResourceExecutor&, const std::function<void(ServiceState&)>&)>& service,
		ThreadPoolWebServer& server,
		DWORD timeout
	) :
		stream
		(
			ssl ?
			streams::IOSocketStream::createStream<web::HttpsNetwork>(clientSocket, ssl, context, std::chrono::milliseconds(timeout)) :
			streams::IOSocketStream::createStream<web::HttpNetwork>(clientSocket, std::chrono::milliseconds(timeout))
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
		web::BaseTCPServer& server,
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

		HTTPRequestImplementation request(sessionsManager, server, staticResources, dynamicResources, address, stream);
		HTTPResponseImplementation response;
		std::vector<std::function<void(ServiceState&)>> chain =
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
				HTTPRequestExecutors requestWrapper(&request);
				HTTPResponseExecutors responseWrapper(&response);

				if (std::optional<std::function<void(HTTPRequestExecutors&, HTTPResponseExecutors&)>> threadPoolFunction = executorsManager.service(requestWrapper, responseWrapper, executors))
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
									HTTPRequestExecutors requestWrapper(&request);
									HTTPResponseExecutors responseWrapper(&response);

									(*threadPoolFunction)(requestWrapper, responseWrapper);
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

		for (const std::function<void(ServiceState&)>& task : chain)
		{
			ServiceState state = service(stream, request, response, resourceExecutor, task);

			if (state == ServiceState::success)
			{
				continue;
			}
			else if (state == ServiceState::skipResponse)
			{
				return false;
			}
			else if (state == ServiceState::error)
			{
				return true;
			}
			else
			{
				throw std::runtime_error("Wrong ServiceState: " + std::to_string(static_cast<int>(state)));
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
		SSL* ssl = nullptr;

		try
		{
			if (useHTTPS)
			{
				ssl = SSL_new(context);

				if (!ssl)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__);
				}

				if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
				{
					SSL_free(ssl);

					throw web::exceptions::SslException(__LINE__, __FILE__);
				}

				if (int errorCode = SSL_accept(ssl); errorCode != 1)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__, ssl, errorCode);
				}
			}

			clients.push_back(new Client(ssl, context, clientSocket, address, move(cleanup), &ExecutorServer::serviceRequests, *this, timeout));
		}
		catch (const web::exceptions::SslException& e)
		{
			if (Log::isValid())
			{
				Log::error("SSL exception: {}, ip: {}", "LogHTTPS", e.what(), ip);
			}
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
		std::shared_ptr<threading::ThreadPool> resourcesThreadPool
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
