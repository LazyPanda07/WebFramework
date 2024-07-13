#include <iostream>
#include <fstream>

#ifdef CXX_API
#include "import.h"
#else
#include "Import/WebFramework.h"
#endif

#include "ConsoleArgumentParser.h"

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

#ifdef CXX_API
	framework::utility::initializeWebFramework("WebFramework");
#endif

	framework::utility::Config config(parser.get<std::string>("--config"));
	int64_t port = parser.get<int64_t>("--port");

	config.overrideConfiguration("port", port, true);

	if (parser.get<bool>("--custom_heuristic"))
	{
		config.overrideConfiguration("heuristic", "CustomHeuristic", true);
	}

	if (std::string type = parser.get<std::string>("--type"); type == "server")
	{
		std::vector<std::string> settingsPaths = { "load_balancer_web.json" };

		config.overrideConfiguration("webServerType", "multiThreaded", true);

		config.overrideConfiguration("settingsPaths", settingsPaths, true);
	}
	else
	{
		bool serversHTTPS = parser.get<bool>("--serversHTTPS");
		std::vector<int64_t> listOfServers;

		config.overrideConfiguration("serversHTTPS", serversHTTPS, true);

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
				10001,
			};
		}

		config.overrideConfiguration("127.0.0.1", listOfServers, true);
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
