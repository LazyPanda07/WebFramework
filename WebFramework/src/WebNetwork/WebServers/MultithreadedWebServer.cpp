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
	void MultithreadedWebServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& addr, function<void()>&& cleanup)
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

		streams::IOSocketStream stream
		(
			useHTTPS ?
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPSNetwork>(clientSocket, ssl, context)) :
			make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(clientSocket))
		);
		unordered_map<string, smartPointer<BaseExecutor>> statefulExecutors;
		HTTPResponse response;
		optional<function<void(HTTPRequest&, HTTPResponse&)>> threadPoolFunction;

		while (isRunning)
		{
			try
			{
				HTTPRequest request(sessionsManager, *this, *resources, *resources, databaseManager, addr, stream);

				response.setDefault();

				stream >> request;

				if (stream.eof())
				{
					break;
				}

				executorsManager.service(request, response, statefulExecutors, threadPoolFunction);

				if (response)
				{
					stream << response;
				}

				if (stream.eof())
				{
					break;
				}
			}
			catch (const web::exceptions::WebException&)
			{
				break;
			}
			catch (const exceptions::BadRequestException&) // 400
			{
				resources->badRequestError(response);

				stream << response;
			}
			catch (const file_manager::exceptions::FileDoesNotExistException&) // 404
			{
				resources->notFoundError(response);

				stream << response;
			}
			catch (const exceptions::BaseExecutorException&) // 500
			{
				resources->internalServerError(response);

				stream << response;
			}
			catch (...)	// 500
			{
				resources->internalServerError(response);

				stream << response;
			}
		}
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
