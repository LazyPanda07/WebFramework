#include "MultithreadedWebServer.h"

#include "Log.h"
#include "HTTPSNetwork.h"

#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/BadRequestException.h"
#include "Utility/RouteParameters.h"
#include "Exceptions/SSLException.h"
#include "Utility/Singletons/HTTPSSingleton.h"

#ifndef __LINUX__
#pragma warning(disable: 6387)
#endif

using namespace std;

namespace framework
{
	void MultithreadedWebServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& addr, function<void()>&& cleanup) //-V688
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
		}
		catch (const web::exceptions::SSLException& e)
		{
			if (Log::isValid())
			{
				Log::error("SSL exception: {}, ip: {}", "LogHTTPS", e.what(), ip);
			}

			return;
		}

		streams::IOSocketStream stream
		(
			useHTTPS ?
			make_unique<web::HTTPSNetwork>(clientSocket, ssl, context) :
			make_unique<web::HTTPNetwork>(clientSocket)
		);
		unordered_map<string, unique_ptr<BaseExecutor>> statefulExecutors;
		HTTPResponse response;

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

				executorsManager.service(request, response, statefulExecutors);

				if (response)
				{
					stream << response;
				}

				if (stream.eof())
				{
					break;
				}
			}
			catch (const web::exceptions::WebException& e)
			{
				if (Log::isValid())
				{
					Log::error("Multithreaded serve exception: {}", "Multithreaded", e.what());
				}

				break;
			}
			catch (const exceptions::BadRequestException& e) // 400
			{
				resources->badRequestError(response, &e);

				stream << response;
			}
			catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
			{
				resources->notFoundError(response, &e);

				stream << response;
			}
			catch (const exceptions::BaseExecutorException& e) // 500
			{
				resources->internalServerError(response, &e);

				stream << response;
			}
			catch (const exception& e)
			{
				resources->internalServerError(response, &e);

				stream << response;
			}
			catch (...)	// 500
			{
				resources->internalServerError(response, nullptr);

				stream << response;
			}
		}
	}

	MultithreadedWebServer::MultithreadedWebServer
	(
		const json::JSONParser& configuration,
		const vector<utility::JSONSettingsParser>& parsers,
		const filesystem::path& assets,
		const filesystem::path& pathToTemplates,
		uint64_t cachingSize,
		string_view ip,
		string_view port,
		DWORD timeout,
		const vector<string>& pathToSources
	) :
		BaseTCPServer
		(
			port,
			ip,
			timeout,
			true,
			0,
			false
		),
		IExecutorFunctionality
		(
			configuration,
			assets,
			pathToTemplates,
			cachingSize,
			parsers,
			pathToSources
		)
	{

	}
}
