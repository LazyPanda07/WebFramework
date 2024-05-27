#include "WebFramework.h"

int main(int argc, char** argv)
{
	framework::WebFramework server("default_https_server_config.json");

	server.startServer(true);

	return 0;
}
