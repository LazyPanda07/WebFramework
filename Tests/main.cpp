#include <functional>
#include <random>
#include <chrono>

#include "WebFramework.h"

#include "gtest/gtest.h"

#include "settings.h"

bool useHTTPS;

void startTests(int argc, char** argv)
{
	std::thread([argc, argv]() mutable
		{
			testing::InitGoogleTest(&argc, argv);

			exit(RUN_ALL_TESTS());
		}).detach();
}

int main(int argc, char** argv) try
{
	std::mt19937 random(time(nullptr));

	std::this_thread::sleep_for(std::chrono::seconds(random() % 10));

	framework::WebFramework server(argv[1]);

	useHTTPS = framework::WebFramework::getUseHTTPS();

	server.startServer(true, std::bind(startTests, argc, argv));

	return 0;
}
catch (const web::exceptions::WebException& e)
{
	std::cout << e.what() << ' ' << e.getFile() << ' ' << e.getLine() << ' ' << e.getErrorCode() << std::endl;

	exit(-1);
}
