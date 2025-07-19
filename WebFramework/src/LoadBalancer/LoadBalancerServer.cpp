#include "LoadBalancerServer.h"

#include "IOSocketStream.h"
#include "Exceptions/SSLException.h"
#include "HTTPSNetwork.h"

#include "Utility/Sources.h"
#include "Heuristics/Connections.h"
#include "WebNetwork/HTTPResponseImplementation.h"
#include <Log.h>

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		LoadBalancerServer::ServerData::ServerData(utility::BaseConnectionData&& connectionData, std::unique_ptr<BaseLoadBalancerHeuristic>&& heuristic) noexcept :
			connectionData(move(connectionData)),
			heuristic(move(heuristic))
		{

		}

		void LoadBalancerServer::clientConnection(const string& ip, SOCKET clientSocket, sockaddr addr, function<void()>& cleanup) //-V688
		{
			static mutex dataMutex;
			const ServerData* serveData = nullptr;

			{
				unique_lock<mutex> lock(dataMutex);

				serveData = &*min_element
				(
					allServers.begin(), allServers.end(),
					[](const ServerData& left, const ServerData& right)
					{
						return (*left.heuristic)() < (*right.heuristic)();
					}
				);

				serveData->heuristic->onStart();
			}

			const auto& [connectionData, heuristic] = *serveData;
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

			streams::IOSocketStream clientStream = ssl ?
				streams::IOSocketStream::createStream<web::HTTPSNetwork>(clientSocket, ssl, context) :
				streams::IOSocketStream::createStream<web::HTTPNetwork>(clientSocket);
			streams::IOSocketStream serverStream = serversHTTPS ?
				streams::IOSocketStream::createStream<web::HTTPSNetwork>(connectionData.ip, connectionData.port, timeout) :
				streams::IOSocketStream::createStream<web::HTTPNetwork>(connectionData.ip, connectionData.port, timeout);

			while (isRunning)
			{
				string request;
				string response;

				if (clientStream.eof())
				{
					if (Log::isValid())
					{
						Log::info("LoadBalancer client eof", "LogLoadBalancer");
					}

					break;
				}

				try
				{
					clientStream >> request;
				}
				catch (const web::exceptions::WebException& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer client request web exception: {}", "LogLoadBalancer", e.what());
					}

					break;
				}
				catch (const exception& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer client request exception: {}", "LogLoadBalancer", e.what());
					}

					break;
				}
				catch (...)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer client request unexpected error", "LogLoadBalancer");
					}

					break;
				}

				if (serverStream.eof())
				{
					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->internalServerError(wrapper, nullptr);

					clientStream << errorResponse;

					break;
				}

				try
				{
					serverStream << request;
				}
				catch (const web::exceptions::WebException& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer server request web exception: {}", "LogLoadBalancer", e.what());
					}

					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->badGatewayError(wrapper, &e);

					clientStream << errorResponse;

					break;
				}
				catch (const exception& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer server request exception: {}", "LogLoadBalancer", e.what());
					}

					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->internalServerError(wrapper, &e);

					clientStream << errorResponse;

					break;
				}
				catch (...)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer server request unexpected error", "LogLoadBalancer");
					}

					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->internalServerError(wrapper, nullptr);

					clientStream << errorResponse;

					break;
				}

				if (serverStream.eof())
				{
					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->internalServerError(wrapper, nullptr);

					clientStream << errorResponse;

					break;
				}

				try
				{
					serverStream >> response;
				}
				catch (const web::exceptions::WebException& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer server response web exception: {}", "LogLoadBalancer", e.what());
					}

					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->badGatewayError(wrapper, &e);

					clientStream << errorResponse;

					break;
				}
				catch (const exception& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer server response exception: {}", "LogLoadBalancer", e.what());
					}

					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->internalServerError(wrapper, &e);

					clientStream << errorResponse;

					break;
				}
				catch (...)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer server response unexpected error", "LogLoadBalancer");
					}

					HTTPResponseImplementation errorResponse;
					HTTPResponse wrapper(&errorResponse);

					resources->internalServerError(wrapper, nullptr);

					clientStream << errorResponse;

					break;
				}

				if (clientStream.eof())
				{
					break;
				}

				try
				{
					clientStream << response;
				}
				catch (const web::exceptions::WebException& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer client response web exception: {}", "LogLoadBalancer", e.what());
					}

					break;
				}
				catch (const exception& e)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer client response exception: {}", "LogLoadBalancer", e.what());
					}

					break;
				}
				catch (...)
				{
					if (Log::isValid())
					{
						Log::error("LoadBalancer client response unexpected error", "LogLoadBalancer");
					}

					break;
				}
			}

			{
				unique_lock<mutex> lock(dataMutex);

				heuristic->onEnd();
			}
		}

		LoadBalancerServer::LoadBalancerServer
		(
			string_view ip, string_view port, DWORD timeout, bool serversHTTPS,
			string_view heuristicName, const vector<HMODULE>& loadSources,
			const unordered_map<string, vector<int64_t>>& allServers, //-V688
			shared_ptr<ResourceExecutor> resources
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
			resources(resources),
			serversHTTPS(serversHTTPS)
		{
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
						unique_ptr<BaseLoadBalancerHeuristic>(static_cast<BaseLoadBalancerHeuristic*>(heuristicCreateFunction(ip, portString, serversHTTPS)))
					);
				}
			}
		}
	}
}
