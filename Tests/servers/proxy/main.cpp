#include <iostream>
#include <fstream>

#ifdef __LINUX__
#include <unistd.h>
#endif

#include <import.hpp>

#include <ConsoleArgumentParser.h>

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

	framework::utility::initializeWebFramework("WebFramework");

	framework::utility::Config config(parser.get<std::string>("config"));

	int64_t port = parser.get<int64_t>("port");

	config.overrideConfiguration("useHTTPS", parser.get<bool>("useHTTPS"));
	config.overrideConfiguration("port", port);

	framework::WebFramework server(config);

	server.start
	(
		true,
		[]()
		{
			std::cout << "Server is running" << std::endl;
		}
	);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
