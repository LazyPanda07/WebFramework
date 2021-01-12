#include "LoadBalancer.h"

#include "INIParser.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "WebFrameworkConstants.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "INIParser.lib")

using namespace std;

namespace framework
{
	LoadBalancer::LoadBalancer(const filesystem::path& configurationINIFile)
	{
		if (!filesystem::exists(configurationINIFile))
		{
			throw exceptions::FileDoesNotExistException(configurationINIFile.string());
		}

		::utility::INIParser parser(configurationINIFile);
		const unordered_map<string, string>& webFrameworkLoadBalancerSettings = parser.getSectionMapData(ini::webFrameworkLoadBalancerSection, ini::listOfServersKey);

		//TODO: connections to all servers
		for (const auto& [ip, port] : webFrameworkLoadBalancerSettings)
		{

		}
	}
}
