#include "WebFramework.h"

#include "ConsoleArgumentParser.h"

#ifdef __LINUX__
#include <unistd.h> 
#endif

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);
	framework::utility::Config config(parser.get<std::string>("--config"));
	int64_t port = parser.get<int64_t>("--port");
	
	config.overrideConfiguration("port", port, true);

	if(parser.get<bool>("--custom_heuristic"))
	{
		config.overrideConfiguration("heuristic", "CustomHeuristic", true);
	}

	if(std::string type = parser.get<std::string>("--type"); type == "server")
	{
		std::vector<json::utility::jsonObject> settingsPaths;

		json::utility::appendArray("load_balancer_web.json", settingsPaths);

		config.overrideConfiguration("webServerType", "multiThreaded", true);

		config.overrideConfiguration("settingsPaths", settingsPaths, true);
	}
	else
	{
		bool serversHTTPS = parser.get<bool>("--serversHTTPS", false);
		std::vector<json::utility::jsonObject> listOfServers;

		config.overrideConfiguration("serversHTTPS", serversHTTPS, true);

		if(serversHTTPS)
		{
			json::utility::appendArray(10002LL, listOfServers);
			json::utility::appendArray(10003LL, listOfServers);
		}
		else
		{
			json::utility::appendArray(10000LL, listOfServers);
			json::utility::appendArray(10001LL, listOfServers);
		}

		config.overrideConfiguration("127.0.0.1", listOfServers, true);
	}

	framework::WebFramework server(config);

	server.startServer
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
