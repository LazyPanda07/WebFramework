#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "import.h"

std::string getConfiguration()
{
	std::ifstream in("multi_threaded_config.json");
	std::ostringstream os;
	std::string result;

	os << in.rdbuf();

	for (char c : os.str())
	{
		if (c == '\r')
		{
			continue;
		}

		result += c;
	}

	return result;
}

framework::utility::Config createConfig()
{
	return framework::utility::Config("multi_threaded_config.json");
}

TEST(API, ConfigConstructors)
{
	framework::utility::Config configFromPath = createConfig();
	framework::utility::Config configFromStrings(getConfiguration(), std::filesystem::current_path().string());

	ASSERT_EQ(configFromPath.getRawConfiguration(), configFromStrings.getRawConfiguration());
	ASSERT_EQ(configFromPath.getConfiguration(), configFromStrings.getConfiguration());
	ASSERT_EQ(configFromPath.getBasePath(), configFromStrings.getBasePath());
}

TEST(API, ConfigOverrideString)
{
	framework::utility::Config config = createConfig();

	config.overrideConfiguration("webServerType", "threadPool");

	ASSERT_NE(config.getConfiguration().find(R"("webServerType": "threadPool")"), std::string::npos);
}

TEST(API, ConfigOverrideInteger)
{
	framework::utility::Config config = createConfig();

	config.overrideConfiguration("cachingSize", 0LL);

	ASSERT_NE(config.getConfiguration().find(R"("cachingSize": 0)"), std::string::npos);
}

TEST(API, ConfigOverrideBool)
{
	framework::utility::Config config = createConfig();

	config.overrideConfiguration("usingLogging", false);

	ASSERT_NE(config.getConfiguration().find(R"("usingLogging": false)"), std::string::npos);
}

TEST(API, ConfigOverrideStringArray)
{
	framework::utility::Config config = createConfig();

	config.overrideConfiguration("loadSources", std::vector<std::string>{ "anotherSource" });

	ASSERT_NE(config.getRawConfiguration(), config.getConfiguration());
}

TEST(API, ConfigOverrideIntegerArray)
{
	framework::utility::Config config = createConfig();

	config.overrideConfiguration("port", std::vector<int64_t>{ 15 });

	ASSERT_NE(config.getRawConfiguration(), config.getConfiguration());
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	framework::utility::initializeWebFramework("WebFramework");

	return RUN_ALL_TESTS();
}
