#include <iostream>
#include <fstream>

#include <import.hpp>

int main(int argc, char** argv) try
{
	framework::utility::initializeWebFramework();

	framework::WebFramework server("config.json");

	server.start(true, []() { std::ofstream("start.txt"); });

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
