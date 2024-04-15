#include <functional>
#include <random>
#include <chrono>

#include "WebFramework.h"

#include "gtest/gtest.h"

#include "settings.h"

bool useHTTPS;

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	useHTTPS = framework::WebFramework::getUseHTTPS();

	exit(RUN_ALL_TESTS());

	return 0;
}
