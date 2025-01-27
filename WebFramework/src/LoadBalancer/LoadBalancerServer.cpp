#include "LoadBalancerServer.h"

#include "IOSocketStream.h"
#include "Exceptions/SSLException.h"
#include "HTTPSNetwork.h"

#include "Utility/Sources.h"
#include "Heuristics/Connections.h"

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

				clientStream >> request;

				if (clientStream.eof())
				{
					break;
				}

				serverStream << request;

				if (serverStream.eof())
				{
					HTTPResponse errorResponse;

					resources->internalServerError(errorResponse, nullptr);

					clientStream << errorResponse;

					break;
				}

				serverStream >> response;

				clientStream << response;
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
			const json::JSONParser& configuration, const filesystem::path& assets, uint64_t cachingSize, const filesystem::path& pathToTemplates
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
			resources(make_shared<ResourceExecutor>(configuration, assets, cachingSize, pathToTemplates)),
			serversHTTPS(serversHTTPS)
		{
			string createHeuristicFunctionName = format("create{}Heuristic", heuristicName);
			createHeuristicFunction heuristicCreateFunction = nullptr;

			if (heuristicName == "Connections")
			{
				heuristicCreateFunction = [](string_view ip, string_view port, bool useHTTPS) -> void* { return new Connections(ip, port, useHTTPS); };
			}
			else
			{
				for (HMODULE source : loadSources)
				{
					if (heuristicCreateFunction = reinterpret_cast<createHeuristicFunction>(utility::load(source, createHeuristicFunctionName)); heuristicCreateFunction)
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
