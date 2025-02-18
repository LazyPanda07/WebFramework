#include "ThreadPoolWebServer.h"

#include "Log.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/NotFoundException.h"
#include "Exceptions/SSLException.h"
#include "Utility/Singletons/HTTPSSingleton.h"
#include "HTTPSNetwork.h"

using namespace std;

namespace framework
{
	ThreadPoolWebServer::Client::Client(SSL* ssl, SSL_CTX* context, SOCKET clientSocket, sockaddr address, function<void()>&& cleanup) :
		stream
		(
			ssl ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>(clientSocket, ssl, context) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>(clientSocket)
		),
		cleanup(move(cleanup)),
		address(address),
		ssl(ssl),
		context(context),
		clientSocket(clientSocket),
		isBusy(false),
		webExceptionAcquired(false)
	{
		/*stream.getNetwork<web::HTTPNetwork>().setLargeBodyHandler
		(
			[this](string_view data) -> bool
			{
				largeRequest->setLargeDataPart(data);

				invoke(method, largeExecutor, *largeRequest, response);

				return !static_cast<bool>(response);
			},
			[&](web::utility::ContainerWrapper& headers)
			{
				largeRequest = make_unique<HTTPRequest>(sessionsManager, *this, *resources, *resources, databaseManager, addr, stream);

				const_cast<web::HTTPParser&>(largeRequest->getParser()).parse(headers.data());

				method = BaseExecutor::methods.at(largeRequest->getMethod());
				largeExecutor = executorsManager.getOrCreateExecutor(*largeRequest, response, statefulExecutors);
			}
		);*/
	}

	bool ThreadPoolWebServer::Client::serve
	(
		SessionsManager& sessionsManager,
		web::BaseTCPServer& server,
		interfaces::IStaticFile& staticResources,
		interfaces::IDynamicFile& dynamicResources,
		sqlite::SQLiteManager& databaseManager,
		ExecutorsManager& executorsManager,
		ResourceExecutor& resourceExecutor,
		threading::ThreadPool& threadPool
	)
	{
		if (stream.eof() || webExceptionAcquired)
		{
			return true;
		}

		if (isBusy)
		{
			return false;
		}

		try
		{
			HTTPRequest request(sessionsManager, server, staticResources, dynamicResources, databaseManager, address, stream);
			HTTPResponse response;

			stream >> request;

			if (stream.eof())
			{
				return true;
			}

			optional<function<void(HTTPRequest&, HTTPResponse&)>> threadPoolFunction = executorsManager.service(request, response, statefulExecutors);

			if (threadPoolFunction)
			{
				isBusy = true;

				threadPool.addTask
				(
					[this, &resourceExecutor, request = move(request), response = move(response), threadPoolFunction = move(threadPoolFunction)]() mutable
					{
						try
						{
							(*threadPoolFunction)(request, response);

							if (response)
							{
								stream << response;
							}
						}
						catch (const web::exceptions::WebException& e)
						{
							if (Log::isValid())
							{
								Log::error("Thread pool serve exception: {}", "LogThreadPool", e.what());
							}

							webExceptionAcquired = true;
						}
						catch (const exceptions::BadRequestException& e) // 400
						{
							resourceExecutor.badRequestError(response, &e);

							stream << response;
						}
						catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
						{
							resourceExecutor.notFoundError(response, &e);

							stream << response;
						}
						catch (const exceptions::NotFoundException& e) // 404
						{
							resourceExecutor.notFoundError(response, &e);

							stream << response;
						}
						catch (const exceptions::BaseExecutorException& e) // 500
						{
							resourceExecutor.internalServerError(response, &e);

							stream << response;
						}
						catch (const exception& e)
						{
							resourceExecutor.internalServerError(response, &e);

							stream << response;
						}
						catch (...) // 500
						{
							resourceExecutor.internalServerError(response, nullptr);

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
				Log::error("Serve exception: {}", "ThreadPool", e.what());
			}

			return true;
		}
		catch (const exceptions::BadRequestException& e) // 400
		{
			HTTPResponse response;

			resourceExecutor.badRequestError(response, &e);

			stream << response;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
		{
			HTTPResponse response;

			resourceExecutor.notFoundError(response, &e);

			stream << response;
		}
		catch (const exceptions::NotFoundException& e) // 404
		{
			HTTPResponse response;

			resourceExecutor.notFoundError(response, &e);

			stream << response;
		}
		catch (const exceptions::BaseExecutorException& e) // 500
		{
			HTTPResponse response;

			resourceExecutor.internalServerError(response, &e);

			stream << response;
		}
		catch (const exception& e)
		{
			HTTPResponse response;

			resourceExecutor.internalServerError(response, &e);

			stream << response;
		}
		catch (...) // 500
		{
			HTTPResponse response;

			resourceExecutor.internalServerError(response, nullptr);

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
				databaseManager,
				executorsManager,
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

			clients.push_back(new Client(ssl, context, clientSocket, address, move(cleanup)));
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
		const vector<utility::JSONSettingsParser>& parsers,
		const filesystem::path& assets,
		const filesystem::path& pathToTemplates,
		uint64_t cachingSize,
		string_view ip,
		string_view port,
		DWORD timeout,
		const vector<string>& pathToSources,
		uint32_t threadCount,
		const utility::AdditionalServerSettings& additionalSettings
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
		IExecutorFunctionality
		(
			configuration,
			assets,
			pathToTemplates,
			cachingSize,
			parsers,
			pathToSources,
			additionalSettings
		),
		threadPool(threadCount ? threadCount : thread::hardware_concurrency())
	{

	}
}
