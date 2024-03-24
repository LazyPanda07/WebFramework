#include <functional>

#include <WebFramework.h>

#include "gtest/gtest.h"

void startTests(int argc, char** argv)
{
	std::async(std::launch::async, [argc, argv]() mutable
		{
			testing::InitGoogleTest(&argc, argv);

			exit(RUN_ALL_TESTS());
		});
}

int main(int argc, char** argv) try
{
	framework::WebFramework server(argv[1]);

	server.startServer(true, std::bind(startTests, argc, argv));

	return 0;
}
catch (const web::exceptions::WebException& e)
{
	std::cout << e.what() << ' ' << e.getFile() << ' ' << e.getLine() << ' ' << e.getErrorCode() << std::endl;

	return -1;
}
