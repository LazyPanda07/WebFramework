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
			HTTPResponseExecutors responseWrapper(&response);

			response.setDefault();

			ServiceState state = this->serviceRequests(stream, request, response, [&stream, &request]() { stream >> request; });

			if (state == ServiceState::error)
			{
				break;
			}
			else if (state == ServiceState::exception || largeBodyHandler.isRunning())
			{
				continue;
			}

			if (stream.eof())
			{
				break;
			}

			HTTPRequestExecutors requestWrapper(&request);

			state = this->serviceRequests(stream, request, response, [this, &requestWrapper, &responseWrapper, &executors]() { executorsManager->service(requestWrapper, responseWrapper, executors); });

			if (state == ServiceState::error)
			{
				break;
			}
			else if (state == ServiceState::exception)
			{
				continue;
			}

			if (response)
			{
				state = this->serviceRequests(stream, request, response, [&stream, &response]() { stream << response; });
			}

			if (state == ServiceState::error)
			{
				break;
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
