#include "LoadBalancer.h"

#include "Exceptions/FileDoesNotExistException.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "INIParser.lib")

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		LoadBalancer::LoadBalancer(const filesystem::path& configurationJSONFile)
		{
			if (!filesystem::exists(configurationJSONFile))
			{
				throw exceptions::FileDoesNotExistException(configurationJSONFile.string());
			}

			json::JSONParser parser(move(ifstream(configurationJSONFile)));

			const string& loadBalancerIp = parser.get<string>(json_settings::loadBalancerIpKey);
			const string& loadBalancerPort = parser.get<string>(json_settings::loadBalancerPortKey);
			int64_t loadBalancerTimeout = parser.get<int64_t>(json_settings::loadBalancerTimeoutKey);
			const smartPointer<json::JSONParser::objectType>& listOfServers = parser.get<smartPointer<json::JSONParser::objectType>>(json_settings::listOfServersKey);
			unordered_map<string, vector<string>> allServers;

			for (const auto& [ip, portsArray] : listOfServers->data)
			{
				auto& data = get<vector<json::utility::objectSmartPointer<json::utility::jsonObject>>>(portsArray);
				vector<string> ports;

				ports.reserve(data.size());

				for (const auto& i : data)
				{
					ports.push_back(get<string>(i->data.front().second));
				}

				allServers.insert(make_pair(ip, move(ports)));
			}

			loadBalancerServer = make_unique<LoadBalancerServer>
				(
					loadBalancerIp,
					loadBalancerPort,
					loadBalancerTimeout,
					allServers
					);
		}

		void LoadBalancer::startLoadBalancer()
		{
			loadBalancerServer->start();
		}

		void LoadBalancer::stopLoadBalancerServer()
		{
			loadBalancerServer->stop();
		}

		bool LoadBalancer::getLoadBalancerState() const
		{
			return loadBalancerServer->serverState();
		}
	}
}
