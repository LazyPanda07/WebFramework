#include "ThreadPoolWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/SSLException.h"
#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "WebNetwork/WebFrameworkHTTPSNetwork.h"
#include "Utility/Singletons/HTTPSSingleton.h"

using namespace std;

namespace framework
{
	void ThreadPoolWebServer::taskImplementation(streams::IOSocketStream&& stream, HTTPRequest&& request, function<void(HTTPRequest&, HTTPResponse&)> executorMethod)
	{
		HTTPResponse response;
		u_long block = 0;

		// ioctlsocket(client.clientSocket, FIONBIO, &block);

		try
		{
			executorMethod(request, response);

			if (response)
			{
				client.stream << response;
			}
		}
		catch (const web::exceptions::WebException& e)
		{
			if (!e.getErrorCode())
			{
				unique_lock<mutex> lock(disconnectMutex);

				data.remove(client.clientIp, client.clientSocket);

				for (auto& i : client.statefulExecutors)
				{
					i.second->destroy();
				}

				disconnectedClients.push_back(client.clientSocket);
			}
		}
		catch (const exceptions::BadRequestException&)	// 400
		{
			resources->badRequestError(response);

			client.stream << response;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException&)	// 404
		{
			resources->notFoundError(response);

			client.stream << response;
		}
		catch (const exceptions::BaseExecutorException&)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}
		catch (...)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}

		block = 1;

		// ioctlsocket(client.clientSocket, FIONBIO, &block);

		client.isBusy = false;
	}

	void ThreadPoolWebServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& addr, function<void()>&& cleanup)
	{
		SSL* ssl = nullptr;

		if (useHTTPS)
		{
			ssl = SSL_new(context);

			if (!ssl)
			{
				return;
			}

			if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
			{
				SSL_free(ssl);

				return;
			}

			if (SSL_accept(ssl) <= 0)
			{
				SSL_free(ssl);

				return;
			}
		}

		HTTPRequest request(sessionsManager, *this, *resources, *resources, databasesManager, client.addr, client.stream);
		HTTPResponse response;
		optional<function<void(HTTPRequest&, HTTPResponse&)>> threadPoolFunction;
		streams::IOSocketStream stream
		(
			useHTTPS ?
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPSNetwork>(clientSocket, ssl, context)) :
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(clientSocket))
		);

		try
		{
			stream >> request;

			executorsManager.service(request, response, client.statefulExecutors, threadPoolFunction);

			if (threadPoolFunction)
			{
				threadPool.addTask
				(
					bind(&ThreadPoolWebServer::taskImplementation, this, move(stream), move(request), *threadPoolFunction)
				);

				return;
			}

			if (response)
			{
				stream << response;
			}
		}
		catch (const web::exceptions::WebException& e)
		{
			if (!e.getErrorCode())
			{
				unique_lock<mutex> lock(disconnectMutex);

				data.remove(client.clientIp, client.clientSocket);

				for (auto& i : client.statefulExecutors)
				{
					i.second->destroy();
				}

				disconnectedClients.push_back(client.clientSocket);
			}
		}
		catch (const exceptions::BadRequestException&)	// 400
		{
			resources->badRequestError(response);

			client.stream << response;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException&)	// 404
		{
			resources->notFoundError(response);

			client.stream << response;
		}
		catch (const exceptions::BaseExecutorException&)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}
		catch (...)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}
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
		this->blockingMode = 1;
	}
}
