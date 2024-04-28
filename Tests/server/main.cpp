#include <filesystem>
#include <chrono>
#include <thread>

#include "WebFramework.h"

int main(int argc, char** argv) try
{
	framework::WebFramework server(argv[1]);

	server.startServer(true, []() { std::ofstream(START_SERVER_FILE); });

	return 0;
}
catch (const web::exceptions::WebException& e)
{
	std::cout << e.what() << std::endl;

	exit(-1);
}
