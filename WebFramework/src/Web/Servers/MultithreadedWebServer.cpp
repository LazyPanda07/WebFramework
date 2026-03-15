#include "Web/Servers/MultithreadedWebServer.h"

#include "Log.h"
#include "HttpsNetwork.h"

#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/BadRequestException.h"
#include <Exceptions/APIException.h>
#include "Utility/RouteParameters.h"
#include "Exceptions/SslException.h"
#include "Utility/Singletons/HTTPSSingleton.h"
#include "Utility/LargeFileHandlers/MultithreadedHandler.h"
#include "Utility/Utils.h"

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
				ssl = this->getNewSsl();

				if (!ssl)
				{
					throw web::exceptions::SslException(__LINE__, __FILE__);
				}

				if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
				{
					SSL_free(ssl);

					throw web::exceptions::SslException(__LINE__, __FILE__);
				}

				if (int errorCode = SSL_accept(ssl); errorCode != 1)
				{
					SSL_free(ssl);

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

			return;
		}

		streams::IOSocketStream stream = this->createServerSideStream(clientSocket, ssl, std::chrono::milliseconds(timeout));
		ExecutorsManager::StatefulExecutors executors;
		HttpResponseImplementation response;
		HttpRequestImplementation request(sessionsManager, *this, *resources, *resources, addr, stream);
		web::HttpNetwork& network = stream.getNetwork<web::HttpNetwork>();
		bool finish = false;

		network.setLargeBodyHandler<utility::MultithreadedHandler>(additionalSettings.largeBodyPacketSize, network, sessionsManager, *this, *resources, *resources, addr, stream, *executorsManager, executors);
		network.setLargeBodySizeThreshold(additionalSettings.largeBodySizeThreshold);

		web::LargeBodyHandler& largeBodyHandler = network.getLargeBodyHandler();

		const std::vector<std::function<void(ServiceState&)>> chain =
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
			[this, &request, &response, &executors](ServiceState& _)
			{
				executorsManager->service(request, response, executors);
			},
			[&stream, &response](ServiceState& _)
			{
				if (response)
				{
					stream << response;
				}
			}
		};
		const void* lastChainTask = &*chain.rbegin();

		while (isRunning)
		{
			response.setDefault();
			const std::function<void(ServiceState&)>* task = &chain.front();

			while (task)
			{
				switch (this->serviceRequests(stream, request, response, *resources, *task))
				{
				case framework::ExecutorServer::ServiceState::success:
					task = task == lastChainTask ? nullptr : task + 1;

					break;

				case framework::ExecutorServer::ServiceState::skipResponse:
					task = nullptr;

					break;

				case framework::ExecutorServer::ServiceState::error:
					finish = true;
					task = nullptr;

					break;
				}
			}

			if (finish)
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
			std::move(executorsSettings),
			pathToSources,
			additionalSettings,
			threadPool
		)
	{

	}
}
