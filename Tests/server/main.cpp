#include <fstream>
#include <filesystem>

#include "WebFramework.h"

#ifdef __LINUX__
#include <unistd.h> 
#endif

int main(int argc, char** argv) try
{
	if (std::filesystem::exists("test_database.sqlite"))
	{
		std::filesystem::remove("test_database.sqlite");
	}

	framework::WebFramework server(argv[1]);

	server.startServer
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
