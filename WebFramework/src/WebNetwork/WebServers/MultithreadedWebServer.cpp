#include "MultithreadedWebServer.h"

#include "Log.h"
#include "HTTPSNetwork.h"

#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/NotFoundException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/BadRequestException.h"
#include <Exceptions/APIException.h>
#include "Utility/RouteParameters.h"
#include "Exceptions/SSLException.h"
#include "Utility/Singletons/HTTPSSingleton.h"
#include "Utility/LargeFileHandlers/MultithreadedHandler.h"

#ifndef __LINUX__
#pragma warning(disable: 6387)
#endif

using namespace std;

namespace framework
{
	void MultithreadedWebServer::clientConnection(const string& ip, SOCKET clientSocket, sockaddr addr, function<void()>& cleanup) //-V688
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

		streams::IOSocketStream stream = useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>(clientSocket, ssl, context) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>(clientSocket);
		unordered_map<string, unique_ptr<BaseExecutor>> statefulExecutors;
		HTTPResponseImplementation response;
		HTTPResponse responseWrapper(&response);
		web::HTTPNetwork& network = stream.getNetwork<web::HTTPNetwork>();

		network.setLargeBodyHandler<utility::MultithreadedHandler>(additionalSettings.largeBodyPacketSize, network, sessionsManager, *this, *resources, *resources, addr, stream, executorsManager, statefulExecutors);
		network.setLargeBodySizeThreshold(additionalSettings.largeBodySizeThreshold);

		web::LargeBodyHandler& largeBodyHandler = network.getLargeBodyHandler();
		
		while (isRunning)
		{
			try
			{
				HTTPRequestImplementation request(sessionsManager, *this, *resources, *resources, addr, stream);

				response.setDefault();

				stream >> request;

				if (largeBodyHandler.isRunning())
				{
					continue;
				}

				if (stream.eof())
				{
					break;
				}

				HTTPRequest requestWrapper(&request);

				executorsManager.service(requestWrapper, responseWrapper, statefulExecutors);

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
					Log::error("Multithreaded serve exception: {}", "LogMultithreadedServer", e.what());
				}

				break;
			}
			catch (const exceptions::BadRequestException& e) // 400
			{
				resources->badRequestError(responseWrapper, &e);

				stream << response;
			}
			catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
			{
				resources->notFoundError(responseWrapper, &e);

				stream << response;
			}
			catch (const exceptions::NotFoundException& e) // 404
			{
				resources->notFoundError(responseWrapper, &e);

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
					Log::error("Internal server error: {}", "LogMultithreadedServer", e.what());
				}

				resources->internalServerError(responseWrapper, &e);

				stream << response;
			}
			catch (const exception& e)
			{
				if (Log::isValid())
				{
					Log::error("Internal server error: {}", "LogMultithreadedServer", e.what());
				}

				resources->internalServerError(responseWrapper, &e);

				stream << response;
			}
			catch (...)	// 500
			{
				resources->internalServerError(responseWrapper, nullptr);

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
		const vector<string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings
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
			pathToSources,
			additionalSettings
		)
	{

	}
}
