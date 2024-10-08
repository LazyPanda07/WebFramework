#include <iostream>
#include <fstream>

#ifdef __LINUX__
#include <unistd.h>
#endif

#ifdef CXX_API
#include "import.h"
#else
#include "Import/WebFramework.h"
#endif

#include "ConsoleArgumentParser.h"

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

#ifdef CXX_API
	framework::utility::initializeWebFramework("WebFramework");
#endif

	framework::utility::Config config(parser.get<std::string>("--config"));

	int64_t port = parser.get<int64_t>("--port");

	config.overrideConfiguration("useHTTPS", parser.get<bool>("--useHTTPS"), true);
	config.overrideConfiguration("port", port, true);

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
