#include "gtest/gtest.h"

#include "import.h"

TEST(API, Config)
{
	framework::initializeWebFramework("WebFramework");

	framework::utility::Config config("multi_threaded_config.json");

	ASSERT_EQ(config.getConfiguration(), config.getRawConfiguration());

	config.overrideConfiguration("settingsPaths", std::vector<std::string>{ "load_balancer_web.json" }, true);

	ASSERT_NE(config.getConfiguration(), config.getRawConfiguration());
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
