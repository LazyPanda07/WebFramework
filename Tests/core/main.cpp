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
		std::ifstream in("test.txt");

		if (in.is_open())
		{
			std::cout << (std::ostringstream() << in.rdbuf()).str() << std::endl;
		}
		else
		{
			std::cout << "no file" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return RUN_ALL_TESTS();
}
