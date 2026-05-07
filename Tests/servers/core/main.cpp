#include <iostream>
#include <fstream>
#include <filesystem>

#include <import.hpp>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

int main(int argc, char** argv) try
{
	if (std::filesystem::exists("test_database.sqlite"))
	{
		std::filesystem::remove("test_database.sqlite");
	}

	framework::utility::initializeWebFramework("WebFramework");

	framework::utility::Config config(argv[1]);

	if (char* temp = std::getenv("RUNTIMES"))
	{
		std::string_view runtimes(temp);

		if (runtimes.find("python") != std::string_view::npos)
		{
			config.overrideConfiguration("$[]WebFramework.runtimes.0.enabled", true);
		}

		if (runtimes.find(".net") != std::string_view::npos)
		{
			config.overrideConfiguration("$[]WebFramework.runtimes.1.enabled", true);
		}
	}
	
#ifdef __WITH_ADDRESS_SANITIZER__
	config.overrideConfiguration("$[]WebFramework.runtimes.0.enabled", false);
	config.overrideConfiguration("$[]WebFramework.runtimes.1.enabled", false);
#endif

	framework::WebFramework server(config);
	
	server.start
	(
		true,
		[]() 
		{
			std::cout << "Server is running..." << std::endl;
		}
	);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	std::ofstream("error.txt") << e.what() << std::endl;

	exit(-1);
}
