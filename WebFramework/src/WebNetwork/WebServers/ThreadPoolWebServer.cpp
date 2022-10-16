#include "ThreadPoolWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/SSLException.h"
#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "WebNetwork/WebFrameworkHTTPSNetwork.h"
#include "Utility/Singletons/HTTPSSingleton.h"

using namespace std;

namespace framework
{
	ThreadPoolWebServer::IndividualData::IndividualData() :
		clientSocket(NULL),
		addr(),
		isBusy(false)
	{

	}

	ThreadPoolWebServer::IndividualData::IndividualData(SOCKET clientSocket, const sockaddr& addr, SSL* ssl, SSL_CTX* context) :
		clientSocket(clientSocket),
		addr(addr),
		clientIp(getClientIpV4(this->addr)),
		stream
		(
			ssl ? 
			make_unique<buffers::IOSocketBuffer>(make_unique<framework::WebFrameworkHTTPSNetwork>(clientSocket, ssl, context)) :
			make_unique<buffers::IOSocketBuffer>(make_unique<framework::WebFrameworkHTTPNetwork>(clientSocket))
		),
		isBusy(false)
	{

	}

	ThreadPoolWebServer::IndividualData::IndividualData(IndividualData&& other) noexcept :
		IndividualData()
	{
		clientSocket = other.clientSocket;
		addr = other.addr;
		clientIp = move(other.clientIp);
		statefulExecutors = move(other.statefulExecutors);
		stream = move(other.stream);
		isBusy = other.isBusy;
	}

	void ThreadPoolWebServer::taskImplementation(HTTPRequest& request, SOCKET socket, function<void(HTTPRequest&&, HTTPResponse&)> executorMethod, vector<SOCKET>& disconnectedClients, shared_ptr<ResourceExecutor>& resourceExecutor)
	{
		HTTPResponse response;
		IndividualData& client = *clients[socket];

		try
		{
			executorMethod(move(request), response);

			client.stream << response;
		}
		catch (const web::exceptions::WebException& e)
		{
			if (!e.getErrorCode())
			{
				unique_lock<mutex> lock(disconnectMutex);

				data.erase(client.clientIp);

				for (auto& i : client.statefulExecutors)
				{
					i.second->destroy();
				}

				disconnectedClients.push_back(client.clientSocket);
			}
		}
		catch (const exceptions::BadRequestException&)	// 400
		{
			resourceExecutor->badRequestError(response);

			client.stream << response;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException&)	// 404
		{
			resourceExecutor->notFoundError(response);

			client.stream << response;
		}
		catch (const exceptions::BaseExecutorException&)	//500
		{
			resourceExecutor->internalServerError(response);

			client.stream << response;
		}
		catch (...)	//500
		{
			resourceExecutor->internalServerError(response);

			client.stream << response;
		}

		client.isBusy = false;
	}

	void ThreadPoolWebServer::mainCycle(IndividualData& client, vector<SOCKET>& disconnectedClients, shared_ptr<ResourceExecutor>& resourceExecutor)
	{
		HTTPRequest request(sessionsManager, *this, *resourceExecutor, *resourceExecutor, databasesManager, client.addr);
		HTTPResponse response;
		optional<function<void(HTTPRequest&&, HTTPResponse&)>> threadPoolFunction;

		try
		{
			client.stream >> request;

			executorsManager.service(move(request), response, client.statefulExecutors, threadPoolFunction);

			if (threadPoolFunction)
			{
				client.isBusy = true;

#pragma warning(push)
#pragma warning(disable: 26800)

				threadPool.addTask
				(
					bind(&ThreadPoolWebServer::taskImplementation, this, move(request), client.clientSocket, *threadPoolFunction, disconnectedClients, resourceExecutor)
				);

#pragma warning(pop)

				return;
			}

			client.stream << response;
		}
		catch (const web::exceptions::WebException& e)
		{
			if (!e.getErrorCode())
			{
				unique_lock<mutex> lock(disconnectMutex);

				data.erase(client.clientIp);

				for (auto& i : client.statefulExecutors)
				{
					i.second->destroy();
				}

				disconnectedClients.push_back(client.clientSocket);
			}
		}
		catch (const exceptions::BadRequestException&)	// 400
		{
			resourceExecutor->badRequestError(response);

			client.stream << response;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException&)	// 404
		{
			resourceExecutor->notFoundError(response);

			client.stream << response;
		}
		catch (const exceptions::BaseExecutorException&)	//500
		{
			resourceExecutor->internalServerError(response);

			client.stream << response;
		}
		catch (...)	//500
		{
			resourceExecutor->internalServerError(response);

			client.stream << response;
		}
	}

	void ThreadPoolWebServer::receiveConnections()
	{
		vector<SOCKET> disconnectedClients;
		SSL_CTX* context = nullptr;
		utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();
		bool useHTTPS = httpsSettings.getUseHTTPS();
		u_long block = 0;
		shared_ptr<ResourceExecutor> resourceExecutor = resources.lock();

		if (useHTTPS)
		{
			context = SSL_CTX_new(TLS_server_method());

			try
			{
				if (!context)
				{
					throw web::exceptions::SSLException();
				}

				if (SSL_CTX_use_certificate_file(context, httpsSettings.getPathToCertificate().string().data(), SSL_FILETYPE_PEM) <= 0)
				{
					throw web::exceptions::SSLException();
				}

				if (SSL_CTX_use_PrivateKey_file(context, httpsSettings.getPathToKey().string().data(), SSL_FILETYPE_PEM) <= 0)
				{
					throw web::exceptions::SSLException();
				}
			}
			catch (const web::exceptions::SSLException& e)
			{
				cout << e.what() << endl;

				exit(-1);
			}
		}

		while (isRunning)
		{
			sockaddr addr;
			int addrlen = sizeof(addr);

			SOCKET clientSocket = accept(listenSocket, &addr, &addrlen);

			if (isRunning && clientSocket != INVALID_SOCKET)
			{
				setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));

				ioctlsocket(clientSocket, FIONBIO, &block);

				data.insert(getClientIpV4(addr), clientSocket);

				SSL* ssl = nullptr;

				if (useHTTPS)
				{
					ssl = SSL_new(context);

					if (!ssl)
					{
						continue;
					}

					if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
					{
						SSL_free(ssl);

						continue;
					}

					if (SSL_accept(ssl) <= 0)
					{
						SSL_free(ssl);

						continue;
					}
				}

				ioctlsocket(clientSocket, FIONBIO, &blockingMode);

				this->clientConnectionImplementation(clientSocket, addr, ssl, context);
			}

			for (auto& [clientSocket, client] : clients)
			{
				if (!client->isBusy)
				{
					this->mainCycle(*client, disconnectedClients, resourceExecutor);
				}
			}

			for (SOCKET clientSocket : disconnectedClients)
			{
				auto it = clients.find(clientSocket);

				delete it->second;

				clients.erase(it);
			}

			disconnectedClients.clear();
		}

		for (const auto& [ip, _] : data.getClients())
		{
			this->pubDisconnect(ip);
		}

		if (useHTTPS)
		{
			SSL_CTX_free(context);
		}
	}

	void ThreadPoolWebServer::clientConnectionImplementation(SOCKET clientSocket, sockaddr addr, SSL* ssl, SSL_CTX* context)
	{
		if (clients.find(clientSocket) == clients.end())
		{
			clients.insert(make_pair(clientSocket, new IndividualData(clientSocket, addr, ssl, context)));

			return;
		}
	}

	void ThreadPoolWebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		throw exceptions::NotImplementedException();
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
