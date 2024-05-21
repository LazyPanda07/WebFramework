#include "LoadBalancerServer.h"

#include "IOSocketStream.h"

#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "Utility/Sources.h"

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

		void LoadBalancerServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& addr, function<void()>&& cleanup)
		{
			while (isRunning)
			{
				
			}
		}

		LoadBalancerServer::LoadBalancerServer
		(
			string_view ip, string_view port, DWORD timeout,
			string_view heuristicName, const vector<HMODULE>& loadSources,
			const unordered_map<string, vector<string>>& allServers
		) :
			BaseTCPServer
			(
				port,
				ip,
				timeout,
				true,
				0,
				false
			)
		{
			string createHeuristicFunctionName = format("create{}Heuristic", heuristicName);
			void*(*heuristicCreateFunction)() = nullptr;

			for (HMODULE source : loadSources)
			{
				if (heuristicCreateFunction = reinterpret_cast<void*(*)()>(utility::load(source, createHeuristicFunctionName)); heuristicCreateFunction)
				{
					break;	
				}
			}

			if (!heuristicCreateFunction)
			{
				throw runtime_error("Can't find heuristic");
			}

			this->allServers.reserve(allServers.size());

			for (const auto& [ip, ports] : allServers)
			{
				for (const string& port : ports)
				{
					// TODO: timeout

					this->allServers.emplace_back
					(
						utility::BaseConnectionData(ip, port, 0), 
						unique_ptr<BaseLoadBalancerHeuristic>(static_cast<BaseLoadBalancerHeuristic*>(heuristicCreateFunction()));
					);
				}
			}
		}
	}
}
