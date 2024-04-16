#include <chrono>
#include <fstream>

#include "JSONParser.h"

#include "gtest/gtest.h"

#include "settings.h"

bool useHTTPS;

int main(int argc, char** argv)
{
	using namespace std::chrono_literals;

	useHTTPS = json::JSONParser(std::ifstream(argv[1])).getObject("WebFramework").getObject("HTTPS").getBool("useHTTPS");

	std::this_thread::sleep_for(5s);

	testing::InitGoogleTest(&argc, argv);

	exit(RUN_ALL_TESTS());

	return 0;
}
