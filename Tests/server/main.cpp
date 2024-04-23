#include <filesystem>
#include <chrono>
#include <thread>

#include "WebFramework.h"

int main(int argc, char** argv) try
{
	framework::WebFramework server(argv[1]);

	server.startServer();

	std::this_thread::sleep_for(std::chrono::seconds(5));

	std::cout << "Server: " << server.isServerRunning() << std::endl;

	std::this_thread::sleep_for(std::chrono::minutes(2));

	server.stopServer(true);

	return 0;
}
catch (const web::exceptions::WebException& e)
{
	std::cout << e.what() << std::endl;

	exit(-1);
}
