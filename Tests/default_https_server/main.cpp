#include <iostream>

#include "import.h"

int main(int argc, char** argv) try
{
	framework::utility::initializeWebFramework("WebFramework");

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
