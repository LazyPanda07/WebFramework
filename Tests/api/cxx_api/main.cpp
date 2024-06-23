#include "gtest/gtest.h"

#include "Service.h"

TEST(API, Config)
{
	framework::Service& service = framework::Service::createService("WebFramework");
	framework::utility::Config config = service.createConfig("multi_threaded_config.json");

	ASSERT_EQ(config.getConfiguration(), config.getRawConfiguration());

	config.overrideConfiguration("settingsPaths", std::vector<std::string>{ "load_balancer_web.json" }, true);

	ASSERT_NE(config.getConfiguration(), config.getRawConfiguration());
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
