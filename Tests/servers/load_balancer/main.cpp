#include <iostream>
#include <fstream>

#include <import.hpp>

#include <ConsoleArgumentParser.h>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

	framework::utility::initializeWebFramework("WebFramework");

	framework::utility::Config config(parser.getRequired<std::string>("config"));
	int64_t port = parser.get<int64_t>("port");

	config.overrideConfiguration("port", port);

	if (parser.get<bool>("custom_heuristic"))
	{
		config.overrideConfiguration("$[]LoadBalancer.heuristic.name", "CustomHeuristic");
	}

	if (char* temp = std::getenv("RUNTIMES"))
	{
		std::string_view runtimes(temp);

		if (runtimes.find("python") != std::string_view::npos)
		{
			config.overrideConfiguration("$[]WebFramework.runtimes.0.enabled", true);
		}

		if (runtimes.find(".net") != std::string_view::npos)
		{
			config.overrideConfiguration("$[]WebFramework.runtimes.1.enabled", true);
		}
	}

	if (std::string type = parser.get<std::string>("type"); type == "server")
	{
		std::vector<std::string> settingsPaths = { "load_balancer_web.json" };

		config.overrideConfiguration("webServerType", "multiThreaded");

		config.overrideConfiguration("settingsPaths", settingsPaths);
	}
	else
	{
		bool serversHTTPS = parser.get<bool>("serversHTTPS");
		std::vector<int64_t> listOfServers;

		config.overrideConfiguration("serversHTTPS", serversHTTPS);

		if (serversHTTPS)
		{
			listOfServers =
			{
				10002,
				10003
			};
		}
		else
		{
			listOfServers =
			{
				10000,
				10001
			};
		}

		config.overrideConfiguration("127.0.0.1", listOfServers);
	}

#ifdef __WITH_ADDRESS_SANITIZER__
	config.overrideConfiguration("$[]WebFramework.runtimes.0.enabled", false);
	config.overrideConfiguration("$[]WebFramework.runtimes.1.enabled", false);
#endif

	framework::WebFramework server(config);

	server.start
	(
		true,
		[]()
		{
			std::cout << "Server is running..." << std::endl;
		}
	);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
