#include "LoadBalancer/LoadBalancerServer.h"

#include <Exceptions/SslException.h>
#include <HttpsNetwork.h>
#include <Log.h>

#include "Web/HttpResponseImplementation.h"
#include "Utility/Stopwatch.h"
#include "Framework/WebFrameworkConstants.h"
#include "Utility/Utils.h"
#include "Framework/WebFramework.h"

#include "LoadBalancer/Heuristics/Connections.h"
#include "LoadBalancer/Heuristics/CXXHeuristic.h"
#include "LoadBalancer/Heuristics/CCHeuristic.h"

#ifdef __WITH_PYTHON_EXECUTORS__
#include "LoadBalancer/Heuristics/PythonHeuristic.h"
#endif

#ifdef __WITH_DOTNET_EXECUTORS__
#include "LoadBalancer/Heuristics/CSharpHeuristic.h"
#endif

namespace framework::load_balancer
{
	LoadBalancerServer::ServerData::ServerData(utility::ConnectionData&& connectionData, std::unique_ptr<LoadBalancerHeuristic>&& heuristic) noexcept :
		connectionData(std::move(connectionData)),
		heuristic(std::move(heuristic))
	{

	}

	LoadBalancerServer::LoadBalancerRequest::LoadBalancerRequest(streams::IOSocketStream&& clientStream, streams::IOSocketStream&& serverStream, LoadBalancerHeuristic* heuristic, std::function<void()>&& cleanup) :
		clientStream(std::move(clientStream)),
		serverStream(std::move(serverStream)),
		cleanup(std::move(cleanup)),
		heuristic(heuristic),
		currentState(State::receiveServerResponse)
	{

	}

	LoadBalancerServer::LoadBalancerRequest::~LoadBalancerRequest()
	{
		if (cleanup)
		{
			cleanup();
		}
	}

	bool LoadBalancerServer::receiveClientRequest(LoadBalancerRequest& request, std::string& httpRequest)
	{
		try
		{
			request.clientStream >> httpRequest;
		}
		catch (const web::exceptions::WebException& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::receiveClientRequestWebError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::receiveClientRequestInternalError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::receiveClientRequestUnexpectedError, logging::category::loadBalancer>();
			}

			return false;
		}

		return true;
	}

	bool LoadBalancerServer::sendClientRequest(LoadBalancerRequest& request, std::string& httpRequest)
	{
		try
		{
			request.serverStream << httpRequest;
		}
		catch (const web::exceptions::WebException& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sendClientRequestWebError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sendClientRequestInternalError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sendClientRequestUnexpectedError, logging::category::loadBalancer>();
			}

			return false;
		}

		return true;
	}

	bool LoadBalancerServer::receiveServerResponse(LoadBalancerRequest& request, std::string& httpResponse)
	{
		try
		{
			request.serverStream >> httpResponse;
		}
		catch (const web::exceptions::WebException& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::receiveServerRequestWebError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::receiveServerRequestInternalError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::receiveServerRequestUnexpectedError, logging::category::loadBalancer>();
			}

			return false;
		}

		return true;
	}

	bool LoadBalancerServer::sendClientResponse(LoadBalancerRequest& request, const std::string& httpResponse)
	{
		try
		{
			request.clientStream << httpResponse;
		}
		catch (const web::exceptions::WebException& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sendServerRequestWebError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sendServerRequestInternalError, logging::category::loadBalancer>(e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::sendServerRequestUnexpectedError, logging::category::loadBalancer>();
			}

			return false;
		}

		return true;
	}

	void LoadBalancerServer::processing(size_t index)
	{
		threading::utility::ConcurrentQueue<LoadBalancerRequest>& queuedRequests = requestQueues[index];
		std::atomic_int64_t& currentSize = processingClients[index];
		std::vector<LoadBalancerRequest> requests;
		utility::Stopwatch stopwatch;

		while (isRunning)
		{
			stopwatch.restart();

			std::vector<size_t> removeIndices;

			for (size_t i = 0; i < requests.size(); i++)
			{
				LoadBalancerRequest& request = requests[i];

				switch (request.currentState)
				{
				case LoadBalancerRequest::State::receiveClientRequest:
					if (request.clientStream.getNetwork().isDataAvailable())
					{
						std::string httpRequest;

						if (LoadBalancerServer::receiveClientRequest(request, httpRequest))
						{
							if (LoadBalancerServer::sendClientRequest(request, httpRequest))
							{
								request.currentState = LoadBalancerRequest::State::receiveServerResponse;
							}
							else
							{
								HttpResponseImplementation response;

								resources->badGatewayError(response);

								utility::processStreamOperation<logging::category::loadBalancer, utility::structs::SendOperation>(request.clientStream, response);
							}
						}
						else
						{
							removeIndices.push_back(i);
						}
					}

					break;

				case LoadBalancerRequest::State::receiveServerResponse:
					if (request.serverStream.getNetwork().isDataAvailable())
					{
						std::string httpResponse;

						if (LoadBalancerServer::receiveServerResponse(request, httpResponse))
						{
							if (LoadBalancerServer::sendClientResponse(request, httpResponse))
							{
								request.currentState = LoadBalancerRequest::State::receiveClientRequest;
							}
							else
							{
								removeIndices.push_back(i);
							}
						}
						else
						{
							HttpResponseImplementation response;

							resources->badGatewayError(response);

							utility::processStreamOperation<logging::category::loadBalancer, utility::structs::SendOperation>(request.clientStream, response);
						}
					}

					break;

				default:
					break;
				}
			}

			for (size_t i = 0; i < removeIndices.size(); i++)
			{
				auto it = requests.begin() + removeIndices[i] - i;

				it->heuristic->onEnd();

				requests.erase(it);

				currentSize--;
			}

			size_t queuedSize = queuedRequests.size();

			for (size_t i = 0; i < queuedSize; i++)
			{
				requests.emplace_back(std::move(*queuedRequests.pop()));
			}

			if (std::chrono::microseconds elapsed = stopwatch.elapsed(); elapsed < threshold)
			{
				std::this_thread::sleep_for(threshold - elapsed);
			}
		}
	}

	std::unique_ptr<LoadBalancerHeuristic> LoadBalancerServer::createAPIHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, std::string_view apiType, utility::LoadSource loadSource) const
	{
		static const std::unordered_map<std::string_view, std::function<std::unique_ptr<LoadBalancerHeuristic>(std::string_view, std::string_view, bool)>> apiHeuristics =
		{
			{ "", [](std::string_view ip, std::string_view port, bool useHTTPS) { return make_unique<Connections>(ip, port, useHTTPS); } },
			{ json_settings::cxxExecutorKey, [heuristicName, &loadSource](std::string_view ip, std::string_view port, bool useHTTPS) { return std::make_unique<CXXHeuristic>(ip, port, useHTTPS, heuristicName, std::get<HMODULE>(loadSource)); } },
			{ json_settings::ccExecutorKey, [heuristicName, &loadSource](std::string_view ip, std::string_view port, bool useHTTPS) { return std::make_unique<CCHeuristic>(ip, port, useHTTPS, heuristicName, std::get<HMODULE>(loadSource)); } },
#ifdef __WITH_PYTHON_EXECUTORS__
			{ json_settings::pythonExecutorKey, [heuristicName, &loadSource](std::string_view ip, std::string_view port, bool useHTTPS) { return std::make_unique<PythonHeuristic>(ip, port, useHTTPS, heuristicName, loadSource); } },
#endif
#ifdef __WITH_DOTNET_EXECUTORS__
			{ json_settings::csharpExecutorKey, [heuristicName, &loadSource](std::string_view ip, std::string_view port, bool useHTTPS) { return std::make_unique<CSharpHeuristic>(ip, port, useHTTPS, heuristicName, loadSource); }}
#endif
		};

		if (auto it = apiHeuristics.find(apiType); it != apiHeuristics.end())
		{
			return (it->second)(ip, port, useHTTPS);
		}

		utility::logAndThrowException<logging::message::cantFindHeuristicType, logging::category::loadBalancer>(apiType);

		return nullptr;
	}

	void LoadBalancerServer::receiveConnections(const std::function<void()>& onStartServer, std::exception** outException)
	{
		threads.reserve(requestQueues.size());

		for (size_t i = 0; i < requestQueues.size(); i++)
		{
			threads.emplace_back(std::async(std::launch::async, &LoadBalancerServer::processing, this, i));
		}

		BaseTCPServer::receiveConnections(onStartServer, outException);
	}

	void LoadBalancerServer::clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup)
	{
		static std::mutex minElementMutex;

		utility::ConnectionData* connectionData = nullptr;
		LoadBalancerHeuristic* heuristic = nullptr;

		{
			std::lock_guard<std::mutex> lock(minElementMutex);
			auto it = min_element
			(
				allServers.begin(), allServers.end(),
				[](const ServerData& left, const ServerData& right)
				{
					return (*left.heuristic)() < (*right.heuristic)();
				}
			);

			connectionData = &it->connectionData;
			heuristic = it->heuristic.get();

			if (Log::isValid())
			{
				Log::info<logging::message::heuristicSelect, logging::category::loadBalancer>(connectionData->ip, connectionData->port, (*heuristic)());
			}

			heuristic->onStart();
		}

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

				if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
				{
					throw web::exceptions::SslException(__LINE__, __FILE__);
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
		
		std::chrono::milliseconds timeoutInMilliseconds(timeout);
		LoadBalancerRequest request
		(
			this->createServerSideStream(clientSocket, ssl, timeoutInMilliseconds),
			serversHTTPS ? streams::IOSocketStream::createStream<web::HttpsNetwork>(connectionData->ip, connectionData->port, timeoutInMilliseconds) : streams::IOSocketStream::createStream<web::HttpNetwork>(connectionData->ip, connectionData->port, timeoutInMilliseconds),
			heuristic,
			std::move(cleanup)
		);
		std::string httpRequest;

		if (LoadBalancerServer::receiveClientRequest(request, httpRequest) && LoadBalancerServer::sendClientRequest(request, httpRequest))
		{
			int64_t minValue = processingClients[0].load(std::memory_order_relaxed);
			size_t minIndex = 0;

			for (size_t i = 1; i < processingClients.size(); i++)
			{
				int64_t value = processingClients[i].load(std::memory_order_relaxed);

				if (value < minValue)
				{
					minValue = value;
					minIndex = i;
				}
			}

			requestQueues[minIndex].push(std::move(request));

			processingClients[minIndex]++;
		}
	}

	LoadBalancerServer::LoadBalancerServer
	(
		std::string_view ip, std::string_view port, DWORD timeout, bool serversHTTPS,
		const json::JsonObject& heuristic, utility::LoadSource loadSource,
		const std::unordered_map<std::string, std::vector<int64_t>>& allServers,
		std::shared_ptr<ResourceExecutor> resources,
		uint32_t processingThreads,
		uint32_t loadBalancingTargetRPS,
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
		requestQueues(processingThreads),
		processingClients(processingThreads),
		resources(resources),
		threshold(decltype(threshold)::period::den / loadBalancingTargetRPS * processingThreads),
		serversHTTPS(serversHTTPS)
	{
		const std::string& heuristicName = heuristic["name"].get<std::string>();
		const std::optional<WebFramework::HttpsData>& httpsData = frameworkInstance.getHttpsData();
		std::string apiType = heuristic[json_settings::apiTypeKey].get<std::string>();

		if (heuristicName == "Connections")
		{
			apiType = "";
		}
		
		this->allServers.reserve(allServers.size());

		std::ranges::for_each(processingClients, [](std::atomic_int64_t& value) { value = 0; });

		for (const auto& [ip, ports] : allServers)
		{
			for (int64_t port : ports)
			{
				std::string portString = std::to_string(port);

				this->allServers.emplace_back
				(
					utility::ConnectionData(ip, portString, timeout),
					this->createAPIHeuristic(ip, portString, static_cast<bool>(httpsData), heuristicName, apiType, loadSource)
				);
			}
		}
	}
}
