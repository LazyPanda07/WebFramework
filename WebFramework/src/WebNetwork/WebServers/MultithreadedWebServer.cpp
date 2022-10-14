#include "MultithreadedWebServer.h"

#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "WebNetwork/WebFrameworkHTTPSNetwork.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/BadRequestException.h"
#include "Utility/RouteParameters.h"
#include "Exceptions/SSLException.h"
#include "Utility/Singletons/HTTPSSingleton.h"

#pragma warning(disable: 6387)

using namespace std;

namespace framework
{
	void MultithreadedWebServer::receiveConnections()
	{
		SSL_CTX* context = nullptr;
		utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();
		bool useHTTPS = httpsSettings.getUseHTTPS();
		int addrlen = sizeof(sockaddr);

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
			SOCKET clientSocket = accept(listenSocket, &addr, &addrlen);

			if (isRunning && clientSocket != INVALID_SOCKET)
			{
				setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));

				ioctlsocket(clientSocket, FIONBIO, &blockingMode);

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

				if (multiThreading)
				{
					thread(&MultithreadedWebServer::clientConnectionImplementation, this, clientSocket, addr, ssl, context).detach();
				}
				else
				{
					this->clientConnectionImplementation(clientSocket, addr, ssl, context);
				}

				this->onConnectionReceive(clientSocket, addr);
			}
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

	void MultithreadedWebServer::clientConnectionImplementation(SOCKET clientSocket, sockaddr addr, SSL* ssl, SSL_CTX* context)
	{
		streams::IOSocketStream stream
		(
			utility::HTTPSSingleton::get().getUseHTTPS() ?
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPSNetwork>(clientSocket, ssl, context)) :
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(clientSocket))
		);
		const string clientIp = getClientIpV4(addr);
		unordered_map<string, smartPointer<BaseExecutor>> statefulExecutors;
		HTTPResponse response;
		optional<function<void(HTTPRequest&&, HTTPResponse&)>> threadPoolFunction;
		shared_ptr<ResourceExecutor> resourceExecutor = resources.lock();

		while (isRunning)
		{
			try
			{
				HTTPRequest request(sessionsManager, *this, *resourceExecutor, *resourceExecutor, databasesManager, addr);

				response.setDefault();

				stream >> request;

				executorsManager.service(move(request), response, statefulExecutors, threadPoolFunction);

				stream << response;
			}
			catch (const web::exceptions::WebException&)
			{
				data.erase(clientIp);

				for (auto& i : statefulExecutors)
				{
					i.second->destroy();
				}

				break;
			}
			catch (const exceptions::BadRequestException&) // 400
			{
				resourceExecutor->badRequestError(response);

				stream << response;
			}
			catch (const file_manager::exceptions::FileDoesNotExistException&) // 404
			{
				resourceExecutor->notFoundError(response);

				stream << response;
			}
			catch (const exceptions::BaseExecutorException&) // 500
			{
				resourceExecutor->internalServerError(response);

				stream << response;
			}
			catch (...)	// 500
			{
				resourceExecutor->internalServerError(response);

				stream << response;
			}
		}
	}

	void MultithreadedWebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		throw exceptions::NotImplementedException();
	}

	MultithreadedWebServer::MultithreadedWebServer(const json::JSONParser& configuration, const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, uint64_t cachingSize, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources) :
		BaseTCPServer
		(
			port,
			ip,
			timeout,
			true,
			0,
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
		)
	{
		
	}
}
