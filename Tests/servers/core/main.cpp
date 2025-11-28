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

#ifdef __WITH_ADDRESS_SANITIZER__
	config.overrideConfiguration("WebFramework.runtimes.1.enabled", false);
#endif

	framework::WebFramework server(config);
	
	server.start
	(
		true,
		[]() 
		{
#ifdef __LINUX__
			pid_t processId = getpid();
#else
			DWORD processId = GetCurrentProcessId();
#endif

			std::ofstream(START_CORE_SERVER_FILE) << processId;
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
