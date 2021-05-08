#include "MultithreadedWebServer.h"

#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "WebNetwork/WebFrameworkHTTPSNetwork.h"
#include "BaseIOSocketStream.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/BadRequestException.h"
#include "Utility/RouteParameters.h"
#include "Exceptions/SSLException.h"
#include "Utility/HTTPSSingleton.h"

#pragma warning(disable: 6387)

using namespace std;

namespace framework
{
	void MultithreadedWebServer::receiveConnections()
	{
		SSL_CTX* context = nullptr;
		utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();
		bool useHTTPS = httpsSettings.getUseHTTPS();

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

				if (SSL_CTX_use_PrivateKey_file(context, httpsSettings.getPathToCertificate().string().data(), SSL_FILETYPE_PEM) <= 0)
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

					if (!SSL_set_fd(ssl, clientSocket))
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

		if (useHTTPS)
		{
			SSL_CTX_free(context);
		}
	}

	void MultithreadedWebServer::clientConnectionImplementation(SOCKET clientSocket, sockaddr addr, SSL* ssl, SSL_CTX* context)
	{
		streams::IOSocketStream stream
		(
			new buffers::IOSocketBuffer
			(
				utility::HTTPSSingleton::get().getUseHTTPS() ?
				static_cast<web::Network*>(new WebFrameworkHTTPSNetwork(clientSocket, ssl, context)) : 
				static_cast<web::Network*>(new WebFrameworkHTTPNetwork(clientSocket))
			)
		);
		const string clientIp = getClientIpV4(addr);
		unordered_map<string, smartPointer<BaseExecutor>> statefulExecutors;
		HTTPResponse response;
		optional<function<void(HTTPRequest&&, HTTPResponse&)>> threadPoolFunction;

		while (true)
		{
			try
			{
				HTTPRequest request(sessionsManager, *this, *resources, *resources, databasesManager, addr);

				response.setDefault();

				stream >> request;

				executorsManager.service(move(request), response, statefulExecutors, threadPoolFunction);

				stream << response;
			}
			catch (const web::WebException&)
			{
				data.erase(clientIp);

				for (auto& i : statefulExecutors)
				{
					i.second->destroy();
				}

				break;
			}
			catch (const exceptions::BadRequestException&)	// 400
			{
				resources->badRequestError(response);

				stream << response;
			}
			catch (const exceptions::FileDoesNotExistException&)	// 404
			{
				resources->notFoundError(response);

				stream << response;
			}
			catch (const exceptions::BaseExecutorException&)	//500
			{
				resources->internalServerError(response);

				stream << response;
			}
			catch (...)	//500
			{
				resources->internalServerError(response);

				stream << response;
			}
		}
	}

	void MultithreadedWebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		throw exceptions::NotImplementedException();
	}

	MultithreadedWebServer::MultithreadedWebServer(const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, bool isCaching, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources) :
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
			parsers,
			assets,
			pathToTemplates,
			isCaching,
			ip,
			port,
			timeout,
			pathToSources
		)
	{
		
	}
}
