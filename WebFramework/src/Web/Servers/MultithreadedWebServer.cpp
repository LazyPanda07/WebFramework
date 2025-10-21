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

namespace framework
{
	void MultithreadedWebServer::clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup)
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
			streams::IOSocketStream::createStream<web::HTTPSNetwork>(clientSocket, ssl, context, std::chrono::milliseconds(timeout)) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>(clientSocket, std::chrono::milliseconds(timeout));
		ExecutorsManager::StatefulExecutors executors;
		HTTPResponseImplementation response;
		HTTPResponseExecutors responseWrapper(&response);
		web::HTTPNetwork& network = stream.getNetwork<web::HTTPNetwork>();

		network.setLargeBodyHandler<utility::MultithreadedHandler>(additionalSettings.largeBodyPacketSize, network, sessionsManager, *this, *resources, *resources, addr, stream, *executorsManager, executors);
		network.setLargeBodySizeThreshold(additionalSettings.largeBodySizeThreshold);

		web::LargeBodyHandler& largeBodyHandler = network.getLargeBodyHandler();
		
		while (isRunning)
		{
			HTTPRequestImplementation request(sessionsManager, *this, *resources, *resources, addr, stream);

			try
			{
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

				HTTPRequestExecutors requestWrapper(&request);

				executorsManager->service(requestWrapper, responseWrapper, executors);

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
			catch (const std::exception& e)
			{
				framework::ExceptionData exceptionData;

				if (request.getExceptionData(exceptionData))
				{
					if (Log::isValid())
					{
						Log::error("Exception from API: {} with response code: {}", exceptionData.logCategory, e.what(), exceptionData.responseCode);
					}

					response.setResponseCode(exceptionData.responseCode);
					response.setBody(exceptionData.errorMessage.data());
				}
				else
				{
					if (Log::isValid())
					{
						Log::error("Internal server error: {}", "LogMultithreadedServer", e.what());
					}

					resources->internalServerError(responseWrapper, &e);
				}

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
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		std::string_view ip,
		std::string_view port,
		DWORD timeout,
		const std::vector<std::string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		std::shared_ptr<threading::ThreadPool> threadPool
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
		ExecutorServer
		(
			configuration,
			move(executorsSettings),
			pathToSources,
			additionalSettings,
			threadPool
		)
	{

	}
}
