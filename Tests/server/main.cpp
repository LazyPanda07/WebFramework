#include <iostream>
#include <fstream>
#include <filesystem>

#ifdef CXX_API
#include "Service.h"
#else
#include "Import/WebFramework.h"
#endif

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

#ifdef CXX_API
	framework::Service& serice = framework::Service::createService("WebFramework");
	framework::WebFramework server = serice.createWebFramework(argv[1]);
#else
	framework::WebFramework server(argv[1]);
#endif
	
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

	exit(-1);
}
