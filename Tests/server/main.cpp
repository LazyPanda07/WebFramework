#include <filesystem>
#include <chrono>
#include <thread>

#include "WebFramework.h"

int main(int argc, char** argv) try
{
	framework::WebFramework server(argv[1]);

	server.startServer();

	while (true)
	{
		if (std::filesystem::exists("stop_server.txt"))
		{
			server.stopServer(true);

			break;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
catch (const web::exceptions::WebException& e)
{
	std::cout << e.what() << std::endl;

	exit(-1);
}
