#include <functional>

#include <WebFramework.h>

#include "gtest/gtest.h"

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
	framework::WebFramework server("config.json");

	server.startServer(true, std::bind(startTests, argc, argv));

	return 0;
}
catch (const web::exceptions::WebException& e)
{
	std::cout << e.what() << ' ' << e.getFile() << ' ' << e.getLine() << ' ' << e.getErrorCode() << std::endl;

	return -1;
}
