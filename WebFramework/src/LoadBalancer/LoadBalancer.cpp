#include "LoadBalancer.h"

#include "INIParser.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "WebFrameworkConstants.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "INIParser.lib")

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		LoadBalancer::LoadBalancer(const filesystem::path& configurationINIFile)
		{
			if (!filesystem::exists(configurationINIFile))
			{
				throw exceptions::FileDoesNotExistException(configurationINIFile.string());
			}

			::utility::INIParser parser(configurationINIFile);
			const unordered_multimap<string, string>& webFrameworkLoadBalancerDataSettings = parser.getSectionData(ini::webFrameworkLoadBalancerSection);

			try
			{
				auto loadBalancerIp = webFrameworkLoadBalancerDataSettings.equal_range(ini::loadBalancerIpKey);
				auto loadBalancerPort = webFrameworkLoadBalancerDataSettings.equal_range(ini::loadBalancerPortKey);
				auto loadBalancerTimeout = webFrameworkLoadBalancerDataSettings.equal_range(ini::loadBalancerTimeoutKey);

				if (loadBalancerIp.first == webFrameworkLoadBalancerDataSettings.end())
				{
					throw out_of_range(::exceptions::cantFindLoadBalancerIp);
				}

				if (loadBalancerPort.first == webFrameworkLoadBalancerDataSettings.end())
				{
					throw out_of_range(::exceptions::cantFindLoadBalancerPort);
				}

				if (loadBalancerTimeout.first == webFrameworkLoadBalancerDataSettings.end())
				{
					throw out_of_range(::exceptions::cantFindLoadBalancerTimeout);
				}

				loadBalancerServer = make_unique<LoadBalancerServer>(
					loadBalancerIp.first->second,
					loadBalancerPort.first->second,
					stoul(loadBalancerTimeout.first->second),
					parser.getSectionMapData(ini::webFrameworkLoadBalancerSection, ini::listOfServersKey)
					);
			}
			catch (const out_of_range&)	//not found settings in unordered_multimap
			{
				throw;
			}
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
