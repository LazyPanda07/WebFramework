#include <functional>

#include <WebFramework.h>

#include "gtest/gtest.h"

void startTests(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	exit(RUN_ALL_TESTS());
}

int main(int argc, char** argv)
{
	framework::WebFramework server("config.json");

	server.startServer(true, std::bind(startTests, argc, argv));

	return 0;
}
