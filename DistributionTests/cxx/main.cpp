#include <iostream>

#include <import.hpp>

int main(int argc, char** argv) try
{
	framework::utility::initializeWebFramework(argv[1]);

	framework::WebFramework server("config.json");

	server.start(true);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
