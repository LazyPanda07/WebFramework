#include "WebFramework.h"

int main(int argc, char** argv) try
{
	framework::WebFramework server("default_https_server_config.json");

	server.startServer(true);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
