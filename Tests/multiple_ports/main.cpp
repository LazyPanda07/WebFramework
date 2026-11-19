#include <iostream>

#include <import.hpp>

int main(int argc, char** argv)
{
	framework::utility::initializeWebFramework();

	framework::utility::Config config("multiple_ports_config.json");

	config.overrideConfiguration("webServerType", argv[1], true);

	framework::WebFramework server(config);

	server.start(true, []() { std::cout << "Server is running..." << std::endl; });

	return 0;
}
