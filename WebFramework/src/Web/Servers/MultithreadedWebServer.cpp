#include "Web/Servers/MultithreadedWebServer.h"

#include <Http/HttpsNetwork.h>

#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/BadRequestException.h"
#include "Exceptions/APIException.h"
#include "Utility/RouteParameters.h"
#include "Exceptions/SslException.h"
#include "Utility/LargeFileHandlers/MultithreadedHandler.h"
#include "Utility/Utils.h"
#include "Framework/WebFramework.h"
#include "ServeLoops/HttpServeLoop.h"

#ifndef __LINUX__
#pragma warning(disable: 6387)
#endif

namespace framework
{
	void MultithreadedWebServer::clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup)
	{
		const std::optional<WebFramework::HttpsData>& httpsData = frameworkInstance.getHttpsData();
		SSL* ssl = nullptr;

		try
		{
			if (httpsData)
			{
				ssl = this->getNewSsl();

				if (!ssl)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__);
				}

				if (int errorCode = SSL_set_fd(ssl, static_cast<int>(clientSocket)); errorCode != 1)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__, ssl, errorCode);
				}

				if (int errorCode = SSL_accept(ssl); errorCode != 1)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__, ssl, errorCode);
				}
			}
		}
		catch (const web::exceptions::SslException& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sslException, logging::category::https>(e.what(), ip);
			}

			closesocket(clientSocket);

			return;
		}

		streams::IOSocketStream stream = this->createServerSideStream(clientSocket, ssl, std::chrono::milliseconds(timeout));
		ExecutorsManager::StatefulExecutors executors;
		HttpResponseImplementation response;
		HttpRequestImplementation request(sessionsManager, *this, *resources, *resources, addr, stream);
		web::http::HttpNetwork& network = stream.getNetwork<web::http::HttpNetwork>();
		bool finish = false;
		std::queue<std::unique_ptr<event::ServeEvent>> events;

		network.setLargeBodyHandler<utility::MultithreadedHandler>(additionalSettings.largeBodyPacketSize, network, sessionsManager, *this, *resources, *resources, addr, stream, *executorsManager, executors);
		network.setLargeBodySizeThreshold(additionalSettings.largeBodySizeThreshold);

		web::LargeBodyHandler& largeBodyHandler = network.getLargeBodyHandler();

		const std::array<std::function<void(ServiceState&)>, 3> chain =
		{
			[&stream, &request, &largeBodyHandler](ServiceState& state)
			{
				stream >> request;

				if (stream.eof()) // request may be empty
				{
					state = ServiceState::error;
				}
				else
				{
					state = largeBodyHandler.isRunning() ? ServiceState::skipResponse : ServiceState::success;
				}
			},
			[this, &request, &response, &executors, &events](ServiceState& _)
			{
				executorsManager->service(request, response, executors, events);
			},
			[&stream, &response](ServiceState& _)
			{
				if (response)
				{
					stream << response;
				}
			}
		};

		serve_loop::HttpServeLoop loop
		(
			stream,
			*resources,
			chain,
			&ExecutorServer::serviceRequests,
			sessionsManager,
			*this,
			addr
		);

		while (isRunning)
		{
			if (loop.run())
			{
				break;
			}
		}
	}

	MultithreadedWebServer::MultithreadedWebServer
	(
		const json::JsonParser& configuration,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		std::string_view ip,
		std::string_view port,
		DWORD timeout,
		const std::vector<std::string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		std::shared_ptr<threading::ThreadPool> threadPool,
		WebFramework& frameworkInstance
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
		BaseWebServer(frameworkInstance),
		ExecutorServer
		(
			configuration,
			std::move(executorsSettings),
			pathToSources,
			additionalSettings,
			threadPool
		)
	{

	}
}
