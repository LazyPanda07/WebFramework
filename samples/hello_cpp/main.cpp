#include <iostream>

#include <import.hpp>

int main(int argc, char** argv) try
{
	framework::utility::initializeWebFramework("WebFramework"); // Load WebFramework shared library

	framework::utility::Config config("configs/config.json"); // Load config.json

	config.overrideBasePath("executors"); // Override base path for loading executors

	framework::WebFramework server(config); // Create server

	server.start(true, []() { std::cout << "Server is running at http://127.0.0.1:8080" << std::endl; }); // Start server and wait

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
