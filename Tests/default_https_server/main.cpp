#include <iostream>

#ifdef PUBLIC_API
#include "import.h"
#else
#include "Import/WebFramework.h"
#endif

int main(int argc, char** argv) try
{
#ifdef PUBLIC_API
	framework::utility::initializeWebFramework("WebFramework");
#endif

	framework::utility::Config config("default_https_server_config.json");
	framework::WebFramework server(config);

	config.overrideConfiguration("userAgentFilter", "CustomUserAgent");

	config.overrideConfiguration("port", 20001);

	framework::WebFramework customUserAgentServer(config);

	customUserAgentServer.start();
	server.start(true);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
