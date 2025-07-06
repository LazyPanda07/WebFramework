#include "ThreadPoolWebServer.h"

#include "Log.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/NotFoundException.h"
#include "Exceptions/SSLException.h"
#include <Exceptions/APIException.h>
#include "Utility/Singletons/HTTPSSingleton.h"
#include "HTTPSNetwork.h"
#include "Utility/LargeFileHandlers/ThreadPoolHandler.h"

using namespace std;

namespace framework
{
	ThreadPoolWebServer::Client::Client(SSL* ssl, SSL_CTX* context, SOCKET clientSocket, sockaddr address, function<void()>&& cleanup, ThreadPoolWebServer& server) :
		stream
		(
			ssl ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>(clientSocket, ssl, context) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>(clientSocket)
		),
		cleanup(move(cleanup)),
		address(address),
		isBusy(false),
		webExceptionAcquired(false)
	{
		web::HTTPNetwork& network = stream.getNetwork<web::HTTPNetwork>();

		network.setLargeBodyHandler<utility::ThreadPoolHandler>
			(
				server.additionalSettings.largeBodyPacketSize, network, server.sessionsManager, server,
				*server.resources, *server.resources,
				address, stream, *server.executorsManager, *statefulExecutors
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

		if (!stream.getNetwork<web::HTTPNetwork>().isDataAvailable())
		{
			return false;
		}

		try
		{
			HTTPRequestImplementation request(sessionsManager, server, staticResources, dynamicResources, address, stream);
			HTTPResponseImplementation response;

			stream >> request;

			if (stream.eof())
			{
				return true;
			}

			if (largeBodyHandler->isRunning())
			{
				return false;
			}

			HTTPRequest requestWrapper(&request);
			HTTPResponse responseWrapper(&response);

			optional<function<void(HTTPRequest&, HTTPResponse&)>> threadPoolFunction = executorsManager.service(requestWrapper, responseWrapper, *statefulExecutors);

			if (threadPoolFunction)
			{
				isBusy = true;

				threadPool.addTask
				(
					[this, &resourceExecutor, request = move(request), response = move(response), threadPoolFunction = move(threadPoolFunction)]() mutable
					{
						HTTPRequest requestWrapper(&request);
						HTTPResponse responseWrapper(&response);

						try
						{
							(*threadPoolFunction)(requestWrapper, responseWrapper);

							if (response)
							{
								stream << response;
							}
						}
						catch (const web::exceptions::WebException& e)
						{
							if (Log::isValid())
							{
								Log::error("Thread pool serve exception: {}", "LogThreadPoolServer", e.what());
							}

							webExceptionAcquired = true;
						}
						catch (const exceptions::BadRequestException& e) // 400
						{
							resourceExecutor.badRequestError(responseWrapper, &e);

							stream << response;
						}
						catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
						{
							resourceExecutor.notFoundError(responseWrapper, &e);

							stream << response;
						}
						catch (const exceptions::NotFoundException& e) // 404
						{
							resourceExecutor.notFoundError(responseWrapper, &e);

							stream << response;
						}
						catch (const exceptions::APIException& e)
						{
							if (Log::isValid())
							{
								Log::error("Exception from API: {} with response code: {}", e.getLogCategory(), e.what(), e.getResponseCode());
							}

							response.setResponseCode(e.getResponseCode());
							response.setBody(e.what());

							stream << response;
						}
						catch (const exceptions::BaseExecutorException& e) // 500
						{
							if (Log::isValid())
							{
								Log::error("Internal server error: {}", "LogThreadPoolServer", e.what());
							}

							resourceExecutor.internalServerError(responseWrapper, &e);

							stream << response;
						}
						catch (const exception& e)
						{
							if (Log::isValid())
							{
								Log::error("Internal server error: {}", "LogThreadPoolServer", e.what());
							}

							resourceExecutor.internalServerError(responseWrapper, &e);

							stream << response;
						}
						catch (...) // 500
						{
							resourceExecutor.internalServerError(responseWrapper, nullptr);

							stream << response;
						}
					},
					[this]() mutable
					{
						isBusy = false;
					}
				);

				return false;
			}
			else if (response)
			{
				stream << response;
			}
		}
		catch (const web::exceptions::WebException& e)
		{
			if (Log::isValid())
			{
				Log::error("Serve exception: {}", "LogThreadPoolServer", e.what());
			}

			return true;
		}
		catch (const exceptions::BadRequestException& e) // 400
		{
			HTTPResponseImplementation response;
			HTTPResponse responseWrapper(&response);

			resourceExecutor.badRequestError(responseWrapper, &e);

			stream << response;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
		{
			HTTPResponseImplementation response;
			HTTPResponse responseWrapper(&response);

			resourceExecutor.notFoundError(responseWrapper, &e);

			stream << response;
		}
		catch (const exceptions::NotFoundException& e) // 404
		{
			HTTPResponseImplementation response;
			HTTPResponse responseWrapper(&response);

			resourceExecutor.notFoundError(responseWrapper, &e);

			stream << response;
		}
		catch (const exceptions::APIException& e)
		{
			HTTPResponseImplementation response;
			HTTPResponse responseWrapper(&response);

			if (Log::isValid())
			{
				Log::error("Exception from API: {} with response code: {}", e.getLogCategory(), e.what(), e.getResponseCode());
			}

			response.setResponseCode(e.getResponseCode());
			response.setBody(e.what());

			stream << response;
		}
		catch (const exceptions::BaseExecutorException& e) // 500
		{
			HTTPResponseImplementation response;
			HTTPResponse responseWrapper(&response);

			if (Log::isValid())
			{
				Log::error("Internal server error: {}", "LogThreadPoolServer", e.what());
			}

			resourceExecutor.internalServerError(responseWrapper, &e);

			stream << response;
		}
		catch (const exception& e)
		{
			HTTPResponseImplementation response;
			HTTPResponse responseWrapper(&response);

			if (Log::isValid())
			{
				Log::error("Internal server error: {}", "LogThreadPoolServer", e.what());
			}

			resourceExecutor.internalServerError(responseWrapper, &e);

			stream << response;
		}
		catch (...) // 500
		{
			HTTPResponseImplementation response;
			HTTPResponse responseWrapper(&response);

			resourceExecutor.internalServerError(responseWrapper, nullptr);

			stream << response;
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

	void ThreadPoolWebServer::clientConnection(const string& ip, SOCKET clientSocket, sockaddr address, function<void()>& cleanup) //-V688
	{
		SSL* ssl = nullptr;

		try
		{
			if (useHTTPS)
			{
				ssl = SSL_new(context);

				if (!ssl)
				{
					throw web::exceptions::SSLException(__LINE__, __FILE__);
				}

				if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
				{
					SSL_free(ssl);

					throw web::exceptions::SSLException(__LINE__, __FILE__);
				}

				if (int errorCode = SSL_accept(ssl); errorCode != 1)
				{
					throw web::exceptions::SSLException(__LINE__, __FILE__, ssl, errorCode);
				}
			}

			clients.push_back(new Client(ssl, context, clientSocket, address, move(cleanup), *this));
		}
		catch (const web::exceptions::SSLException& e)
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
		const json::JSONParser& configuration,
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		string_view ip,
		string_view port,
		DWORD timeout,
		const vector<string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		threading::ThreadPool& threadPool
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
			move(executorsSettings),
			pathToSources,
			additionalSettings,
			threadPool
		),
		threadPool(threadPool)
	{

	}
}
