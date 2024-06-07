#include <fstream>

#include "WebFramework.h"

#include "Log.h"

#ifdef __LINUX__
#include <unistd.h> 
#endif

int main(int argc, char** argv) try
{
	std::cout << __LINE__ << std::endl;

	Log::configure(Log::dateFormat::DMY);

	std::cout << __LINE__ << std::endl;

	framework::WebFramework server(argv[1]);

	std::cout << __LINE__ << std::endl;

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
