#include <iostream>
#include <fstream>

#include "import.hpp"

#include "ConsoleArgumentParser.h"

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

	framework::utility::initializeWebFramework("WebFramework");

	framework::utility::Config config(parser.get<std::string>("--config"));
	int64_t port = parser.get<int64_t>("--port");

	config.overrideConfiguration("port", port);

	if (parser.get<bool>("--custom_heuristic"))
	{
		config.overrideConfiguration("heuristic", "CustomHeuristic");
	}

	if (std::string type = parser.get<std::string>("--type"); type == "server")
	{
		std::vector<std::string> settingsPaths = { "load_balancer_web.json" };

		config.overrideConfiguration("webServerType", "multiThreaded");

		config.overrideConfiguration("settingsPaths", settingsPaths);
	}
	else
	{
		bool serversHTTPS = parser.get<bool>("--serversHTTPS");
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

	framework::WebFramework server(config);

	server.start
	(
		true,
		[port]()
		{
#ifdef __LINUX__
			pid_t processId = getpid();
#else
			DWORD processId = GetCurrentProcessId();
#endif
			switch (port)
			{
			case 9090:
				std::ofstream(START_LOAD_BALANCER_9090_SERVER_FILE) << processId;

				break;

			case 9091:
				std::ofstream(START_LOAD_BALANCER_9091_SERVER_FILE) << processId;

				break;

			case 9092:
				std::ofstream(START_LOAD_BALANCER_9092_SERVER_FILE) << processId;

				break;

			case 9093:
				std::ofstream(START_LOAD_BALANCER_9093_SERVER_FILE) << processId;

				break;

			default:
				break;
			}

		}
	);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
