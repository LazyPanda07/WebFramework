#include <iostream>
#include <fstream>

#ifdef CXX_API
#include "Service.h"
#else
#include "Import/WebFramework.h"
#endif

#include "ConsoleArgumentParser.h"

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

#ifdef CXX_API
	framework::Service& service = framework::Service::createService("WebFramework");
	framework::utility::Config config = service.createConfig(parser.get<std::string>("--config"));
#else
	framework::utility::Config config(parser.get<std::string>("--config"));
#endif

	int64_t port = parser.get<int64_t>("--port");

	config.overrideConfiguration("useHTTPS", parser.get<bool>("--useHTTPS"), true);
	config.overrideConfiguration("port", port, true);

#ifdef CXX_API
	framework::WebFramework server = service.createWebFramework(config);
#else
	framework::WebFramework server(config);
#endif

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
			case 15000:
				std::ofstream(START_PROXY_SERVER_FILE) << processId;

				break;

			case 15001:
				std::ofstream(START_PROXY_HTTPS_SERVER_FILE) << processId;

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
