#include "LoadBalancerServer.h"

#include "Exceptions/SSLException.h"
#include "HTTPSNetwork.h"

#include "Utility/Sources.h"
#include "Heuristics/Connections.h"
#include "Heuristics/CXXHeuristic.h"
#include "Web/HTTPResponseImplementation.h"
#include "Log.h"
#include "Web/HTTPResponseExecutors.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		LoadBalancerServer::ServerData::ServerData(utility::BaseConnectionData&& connectionData, unique_ptr<BaseLoadBalancerHeuristic>&& heuristic) noexcept :
			connectionData(move(connectionData)),
			heuristic(move(heuristic))
		{

		}

		LoadBalancerServer::LoadBalancerRequest::LoadBalancerRequest(streams::IOSocketStream&& clientStream, streams::IOSocketStream&& serverStream, unique_ptr<BaseLoadBalancerHeuristic>& heuristic, function<void()>&& cleanup) :
			clientStream(move(clientStream)),
			serverStream(move(serverStream)),
			cleanup(move(cleanup)),
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

		bool LoadBalancerServer::receiveClientRequest(LoadBalancerRequest& request, string& httpRequest)
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
			catch (const exception& e)
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

		bool LoadBalancerServer::sendClientRequest(LoadBalancerRequest& request, string& httpRequest)
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
			catch (const exception& e)
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

		bool LoadBalancerServer::receiveServerResponse(LoadBalancerRequest& request, string& httpResponse)
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
			catch (const exception& e)
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

		bool LoadBalancerServer::sendClientResponse(LoadBalancerRequest& request, const string& httpResponse)
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
			catch (const exception& e)
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
			vector<LoadBalancerRequest>& requests = requestQueues[index];

			while (isRunning)
			{
				vector<size_t> removeIndices;

				for (size_t i = 0; i < requests.size(); i++)
				{
					LoadBalancerRequest& request = requests[i];

					switch (request.currentState)
					{
					case LoadBalancerRequest::State::receiveClientRequest:
						if (request.clientStream.getNetwork().isDataAvailable())
						{
							string httpRequest;

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
							string httpResponse;

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
				}

				while (true)
				{
					if (optional<LoadBalancerRequest> request = queuedRequests.pop())
					{
						requests.emplace_back(move(*request));
					}
					else
					{
						break;
					}
				}
			}
		}

		unique_ptr<BaseLoadBalancerHeuristic> LoadBalancerServer::createAPIHeuristic(string_view ip, string_view port, bool useHTTPS, string_view apiType) const
		{
			static const unordered_map<string_view, function<unique_ptr<BaseLoadBalancerHeuristic>(string_view, string_view, bool)>> apiHeuristics =
			{
				{ "Connections", [](string_view ip, string_view port, bool useHTTPS) { return make_unique<Connections>(ip, port, useHTTPS); } },
				{ json_settings::cxxExecutorKey, [](string_view ip, string_view port, bool useHTTPS) { return make_unique<CXXHeuristic>(nullptr, ip, port, useHTTPS); } }
			};

			// TODO: exception handling

			return apiHeuristics.at(apiType)(ip, port, useHTTPS);
		}

		void LoadBalancerServer::receiveConnections(const function<void()>& onStartServer, exception** outException)
		{
			threads.reserve(requestQueues.size());

			for (size_t i = 0; i < requestQueues.size(); i++)
			{
				threads.emplace_back(async(launch::async, &LoadBalancerServer::processing, this, i));
			}

			BaseTCPServer::receiveConnections(onStartServer, outException);
		}

		void LoadBalancerServer::clientConnection(const string& ip, SOCKET clientSocket, sockaddr addr, function<void()>& cleanup)
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

			LoadBalancerRequest request
			(
				ssl ? streams::IOSocketStream::createStream<web::HTTPSNetwork>(clientSocket, ssl, context) : streams::IOSocketStream::createStream<web::HTTPNetwork>(clientSocket),
				serversHTTPS ? streams::IOSocketStream::createStream<web::HTTPSNetwork>(connectionData.ip, connectionData.port, timeout) : streams::IOSocketStream::createStream<web::HTTPNetwork>(connectionData.ip, connectionData.port, timeout),
				heuristic,
				move(cleanup)
			);
			string httpRequest;

			if (LoadBalancerServer::receiveClientRequest(request, httpRequest) && LoadBalancerServer::sendClientRequest(request, httpRequest))
			{
				queuedRequests.push(move(request));
			}
		}

		LoadBalancerServer::LoadBalancerServer
		(
			string_view ip, string_view port, DWORD timeout, bool serversHTTPS,
			const json::utility::jsonObject& heuristic, const vector<HMODULE>& loadSources,
			const unordered_map<string, vector<int64_t>>& allServers, //-V688
			shared_ptr<ResourceExecutor> resources,
			size_t processingThreads
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
			resources(resources),
			serversHTTPS(serversHTTPS)
		{
			const string& heuristicName = heuristic.getString("name");
			const string& apiType = heuristic.getString(json_settings::apiTypeKey);

			string createHeuristicFunctionName = format("create{}Heuristic", heuristicName);
			CreateHeuristicFunction heuristicCreateFunction = nullptr;

			if (heuristicName == "Connections")
			{
				heuristicCreateFunction = [](string_view ip, string_view port, bool useHTTPS) -> void* { return new Connections(ip, port, useHTTPS); };
			}
			else
			{
				for (HMODULE source : loadSources)
				{
					if (heuristicCreateFunction = utility::load<CreateHeuristicFunction>(source, createHeuristicFunctionName); heuristicCreateFunction)
					{
						break;
					}
				}

				if (!heuristicCreateFunction)
				{
					throw runtime_error("Can't find heuristic");
				}
			}

			this->allServers.reserve(allServers.size());

			for (const auto& [ip, ports] : allServers)
			{
				for (int64_t port : ports)
				{
					string portString = to_string(port);

					this->allServers.emplace_back
					(
						utility::BaseConnectionData(ip, portString, timeout),
						this->createAPIHeuristic(ip, portString, useHTTPS, apiType)
					);
				}
			}
		}
	}
}
