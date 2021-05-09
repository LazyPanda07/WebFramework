#include "LoadBalancer.h"

#include "JSONParser.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "WebFrameworkConstants.h"

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

			for (const auto& [ip, ports] : listOfServers->data)
			{
				allServers.insert(make_pair(ip, get<vector<string>>(ports)));
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
