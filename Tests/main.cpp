#include <chrono>

#include "WebFramework.h"

#include "gtest/gtest.h"

#include "settings.h"

bool useHTTPS;

int main(int argc, char** argv)
{
	using namespace std::chrono_literals;

	std::this_thread::sleep_for(5s);

	testing::InitGoogleTest(&argc, argv);

	useHTTPS = framework::WebFramework::getUseHTTPS();

	exit(RUN_ALL_TESTS());

	return 0;
}
