#include <fstream>
#include <filesystem>
#include <chrono>

#include "JSONParser.h"

#include "gtest/gtest.h"

#include "settings.h"

bool useHTTPS;

void printLog()
{
	for (const auto& it : std::filesystem::recursive_directory_iterator(std::filesystem::current_path() / "logs"))
	{
		if (it.is_regular_file())
		{
			std::ifstream log(it.path());

			std::cout << (std::ostringstream() << log.rdbuf()).str() << std::endl;
		}
	}
}

int main(int argc, char** argv)
{
	useHTTPS = json::JSONParser(std::ifstream(argv[1])).getObject("WebFramework").getObject("HTTPS").getBool("useHTTPS");

	testing::InitGoogleTest(&argc, argv);

	while (!std::filesystem::exists(START_CORE_SERVER_FILE))
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	int result = RUN_ALL_TESTS();

	if (result)
	{
		printLog();
	}

	return result;
}
