#include <fstream>

#include "JSONParser.h"

#include "gtest/gtest.h"

#include "settings.h"

bool useHTTPS;

int main(int argc, char** argv)
{
	useHTTPS = json::JSONParser(std::ifstream(argv[1])).getObject("WebFramework").getObject("HTTPS").getBool("useHTTPS");

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
