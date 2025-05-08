#include <iostream>

#include "import.h"

int main(int argc, const char** argv) try
{
	framework::utility::initializeWebFramework();

	framework::utility::Config config("config.json"); // Load config.json

	framework::WebFramework server(config); // Create server

	server.start(true, []() { std::cout << "Server is running" << std::endl; }); // Start server and wait

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
catch (...)
{
	std::cerr << "Something went wrong" << std::endl;

	exit(-2);
}
