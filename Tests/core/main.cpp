#include <fstream>
#include <filesystem>
#include <chrono>

#include "JSONParser.h"

#include "gtest/gtest.h"

#include "settings.h"

bool useHTTPS;

int main(int argc, char** argv)
{
	useHTTPS = json::JSONParser(std::ifstream(argv[1])).getObject("WebFramework").getObject("HTTPS").getBool("useHTTPS");

	testing::InitGoogleTest(&argc, argv);

	while (!std::filesystem::exists(START_CORE_SERVER_FILE))
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return RUN_ALL_TESTS();
}
