#include "LoadBalancerServer.h"

#include <Exceptions/SSLException.h>
#include <HTTPSNetwork.h>
#include <Log.h>

#include "Web/HTTPResponseImplementation.h"
#include "Web/HTTPResponseExecutors.h"
#include "Utility/Stopwatch.h"

#include "Heuristics/Connections.h"
#include "Heuristics/CXXHeuristic.h"
#include "Heuristics/CCHeuristic.h"

#ifdef __WITH_PYTHON_EXECUTORS__
#include "Heuristics/PythonHeuristic.h"
#endif

namespace framework::load_balancer
{
	LoadBalancerServer::ServerData::ServerData(utility::BaseConnectionData&& connectionData, std::unique_ptr<BaseLoadBalancerHeuristic>&& heuristic) noexcept :
		connectionData(std::move(connectionData)),
		heuristic(std::move(heuristic))
	{

	}

	LoadBalancerServer::LoadBalancerRequest::LoadBalancerRequest(streams::IOSocketStream&& clientStream, streams::IOSocketStream&& serverStream, std::unique_ptr<BaseLoadBalancerHeuristic>& heuristic, std::function<void()>&& cleanup) :
		clientStream(std::move(clientStream)),
		serverStream(std::move(serverStream)),
		cleanup(std::move(cleanup)),
		heuristic(heuristic.get()),
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
				Log::error("Receiving client request web error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Receiving client request internal error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error("Some unexpected error acquired while getting client request", "LogLoadBalancer");
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
				Log::error("Sending client request web error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Sending client request internal error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error("Some unexpected error acquired while sending client request", "LogLoadBalancer");
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
				Log::error("Receiving server response web error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Receiving server response internal error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error("Some unexpected error acquired while getting server response", "LogLoadBalancer");
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
				Log::error("Sending client response web error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Sending client response internal error: {}", "LogLoadBalancer", e.what());
			}

			return false;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error("Some unexpected error acquired while sending client response", "LogLoadBalancer");
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
								HTTPResponseImplementation response;
								HTTPResponseExecutors responseWrapper(&response);

								resources->badGatewayError(responseWrapper);

								request.clientStream << response;
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
							HTTPResponseImplementation response;
							HTTPResponseExecutors responseWrapper(&response);

							resources->badGatewayError(responseWrapper);

							request.clientStream << response;
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

	std::unique_ptr<BaseLoadBalancerHeuristic> LoadBalancerServer::createAPIHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, std::string_view apiType, utility::LoadSource loadSource) const
	{
		static const std::unordered_map<std::string_view, std::function<std::unique_ptr<BaseLoadBalancerHeuristic>(std::string_view, std::string_view, bool)>> apiHeuristics =
		{
			{ "", [](std::string_view ip, std::string_view port, bool useHTTPS) { return make_unique<Connections>(ip, port, useHTTPS); } },
			{ json_settings::cxxExecutorKey, [heuristicName, &loadSource](std::string_view ip, std::string_view port, bool useHTTPS) { return std::make_unique<CXXHeuristic>(ip, port, useHTTPS, heuristicName, std::get<HMODULE>(loadSource)); } },
			{ json_settings::ccExecutorKey, [heuristicName, &loadSource](std::string_view ip, std::string_view port, bool useHTTPS) { return std::make_unique<CCHeuristic>(ip, port, useHTTPS, heuristicName, std::get<HMODULE>(loadSource)); } },
#ifdef __WITH_PYTHON_EXECUTORS__
			{ json_settings::pythonExecutorKey, [heuristicName, &loadSource](std::string_view ip, std::string_view port, bool useHTTPS) { return std::make_unique<PythonHeuristic>(ip, port, useHTTPS, heuristicName, loadSource); } },
#endif
		};

		if (auto it = apiHeuristics.find(apiType); it != apiHeuristics.end())
		{
			return (it->second)(ip, port, useHTTPS);
		}

		throw std::runtime_error(format("Can't find heuristic type for {}", apiType));

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
		auto& [connectionData, heuristic] = *min_element
		(
			allServers.begin(), allServers.end(),
			[](const ServerData& left, const ServerData& right)
			{
				return (*left.heuristic)() < (*right.heuristic)();
			}
		);

		heuristic->onStart();

		SSL* ssl = nullptr;

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

		std::chrono::milliseconds timeoutInMilliseconds(timeout);
		LoadBalancerRequest request
		(
			ssl ? streams::IOSocketStream::createStream<web::HTTPSNetwork>(clientSocket, ssl, context, timeoutInMilliseconds) : streams::IOSocketStream::createStream<web::HTTPNetwork>(clientSocket, timeoutInMilliseconds),
			serversHTTPS ? streams::IOSocketStream::createStream<web::HTTPSNetwork>(connectionData.ip, connectionData.port, timeoutInMilliseconds) : streams::IOSocketStream::createStream<web::HTTPNetwork>(connectionData.ip, connectionData.port, timeoutInMilliseconds),
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
		const json::utility::jsonObject& heuristic, utility::LoadSource loadSource,
		const std::unordered_map<std::string, std::vector<int64_t>>& allServers,
		std::shared_ptr<ResourceExecutor> resources,
		uint32_t processingThreads,
		uint32_t loadBalancingTargetRPS
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
		requestQueues(processingThreads),
		processingClients(processingThreads, 0),
		resources(resources),
		threshold(decltype(threshold)::period::den / loadBalancingTargetRPS * processingThreads),
		serversHTTPS(serversHTTPS)
	{
		const std::string& heuristicName = heuristic.getString("name");
		std::string apiType = heuristic.getString(json_settings::apiTypeKey);

		if (heuristicName == "Connections")
		{
			apiType = "";
		}

		this->allServers.reserve(allServers.size());

		for (const auto& [ip, ports] : allServers)
		{
			for (int64_t port : ports)
			{
				std::string portString = std::to_string(port);

				this->allServers.emplace_back
				(
					utility::BaseConnectionData(ip, portString, timeout),
					this->createAPIHeuristic(ip, portString, useHTTPS, heuristicName, apiType, loadSource)
				);
			}
		}
	}
}
