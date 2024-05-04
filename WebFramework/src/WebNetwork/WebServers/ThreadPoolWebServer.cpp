#include "ThreadPoolWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/SSLException.h"
#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "WebNetwork/WebFrameworkHTTPSNetwork.h"
#include "Utility/Singletons/HTTPSSingleton.h"

using namespace std;

namespace framework
{
	ThreadPoolWebServer::Client::Client(SSL* ssl, SSL_CTX* context, SOCKET clientSocket, const sockaddr& address, function<void()>&& cleanup) :
		stream
		(
			ssl ?
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPSNetwork>(clientSocket, ssl, context)) :
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(clientSocket))
		),
		cleanup(move(cleanup)),
		address(address),
		ssl(ssl),
		context(context),
		clientSocket(clientSocket),
		isBusy(false),
		webExceptionAcquired(false)
	{

	}

	bool ThreadPoolWebServer::Client::clientServe
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

		optional<function<void(HTTPRequest&, HTTPResponse&)>> threadPoolFunction;
		HTTPResponse response;

		try
		{
			HTTPRequest request(sessionsManager, server, staticResources, dynamicResources, databaseManager, address, stream);

			response.setDefault();

			stream >> request;

			if (stream.eof())
			{
				return true;
			}

			executorsManager.service(request, response, statefulExecutors, threadPoolFunction);

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

							stream << response;
						}
						catch (const web::exceptions::WebException&)
						{
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

			if (response)
			{
				stream << response;
			}
		}
		catch (const web::exceptions::WebException&)
		{
			return true;
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

		return stream.eof();
	}

	ThreadPoolWebServer::Client::~Client()
	{
		if (cleanup)
		{
			cleanup();
		}
	}

	void ThreadPoolWebServer::mainLoop()
	{
		while (isRunning)
		{
			unique_lock<mutex> lock(clientsMutex);

			size_t size = clients.size();
			size_t swaps = 0;

			for (size_t i = 0; i < size;)
			{
				Client& client = clients[i++];

				bool finished = client.clientServe
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
					Client& lastClient = clients[size - 1];

					if (&client != &lastClient)
					{
						swap(client, lastClient);

						swaps++;
						i--;
						size--;
					}
				}
			}

			clients.erase(clients.end() - swaps, clients.end());
		}
	}

	void ThreadPoolWebServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& address, function<void()>&& cleanup)
	{
		SSL* ssl = nullptr;

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

		unique_lock<mutex> lock(clientsMutex);

		clients.emplace_back(ssl, context, clientSocket, address, move(cleanup));
	}

	ThreadPoolWebServer::ThreadPoolWebServer(const json::JSONParser& configuration, const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, uint64_t cachingSize, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources, uint32_t threadCount) :
		BaseTCPServer
		(
			port,
			ip,
			timeout,
			false,
			1,
			false
		),
		BaseWebServer
		(
			configuration,
			parsers,
			assets,
			pathToTemplates,
			cachingSize,
			ip,
			port,
			timeout,
			pathToSources
		),
		threadPool(threadCount ? threadCount : thread::hardware_concurrency())
	{

	}

	void ThreadPoolWebServer::start(bool wait, const function<void()>& onStartServer)
	{
		isRunning = true;

		thread(&ThreadPoolWebServer::mainLoop, this).detach();

		BaseWebServer::start(wait, onStartServer);
	}
}
