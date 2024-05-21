#include "LoadBalancerServer.h"

#include "IOSocketStream.h"

#include "WebNetwork/WebFrameworkHTTPNetwork.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		void LoadBalancerServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& addr, function<void()>&& cleanup)
		{

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
			for (const auto& [ip, ports] : allServers)
			{
				for (const auto& port : ports)
				{
					this->allServers.emplace_back(ip, port);
				}
			}

			for (HMODULE source : loadSources)
			{
				if (auto heuristicCreateFunction = reinterpret_cast<BaseLoadBalancerHeuristic * (*)()>(utility::load(source, heuristicName)); heuristicCreateFunction)
				{
					heuristic = unique_ptr<BaseLoadBalancerHeuristic>(heuristicCreateFunction());
				}
			}

			if (!heuristic)
			{
				throw runtime_error("Can't find heuristic");
			}
		}
	}
}
