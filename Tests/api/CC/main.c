#include "gtest/gtest.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "import.h"

#ifdef __LINUX__
#include <unistd.h>
#else
#include <Windows.h>
#endif

const char* getConfiguration()
{
	FILE* in = fopen("multi_threaded_config.json", "r");
	size_t size = 0;
	size_t index = 0;
	char* data = NULL;
	int c;

	while ((c = fgetc(in)) != EOF)
	{
		if (c != '\r')
		{
			size++;
		}
	}

	data = malloc(size + 1);

	data[size] = 0;

	fseek(in, 0, SEEK_SET);

	while ((c = fgetc(in)) != EOF)
	{
		if (c != '\r')
		{
			data[index++] = c;
		}
	}

	fclose(in);

	return data;
}

Config createConfig()
{
	Config result;

	createConfigFromPath("multi_threaded_config.json", &result);

	return result;
}

TEST(API, ConfigConstructors)
{
	Config configFromPath = createConfig();
	Config configFromStrings;
	const char* data = getConfiguration();
	char* currentDirectory[4096];

	memset(currentDirectory, 0, sizeof(currentDirectory));

#ifdef __LINUX__
	getcwd(currentDirectory, sizeof(currentDirectory));
#else
	GetCurrentDirectory(sizeof(currentDirectory), currentDirectory);
#endif
	
	createConfigFromString(data, currentDirectory, &configFromStrings);

	free(data);

	{
		WebFrameworkString fromPathString;
		WebFrameworkString fromStringsString;

		getConfiguration(configFromPath, &fromPathString);
		getConfiguration(configFromStrings, &fromStringsString);

		ASSERT_FALSE(strcmp(getDataFromString(fromPathString), getDataFromString(fromStringsString)));

		deleteWebFrameworkObject(fromPathString);
		deleteWebFrameworkObject(fromStringsString);
	}

	{
		WebFrameworkString fromPathString;
		WebFrameworkString fromStringsString;

		getBasePath(configFromPath, &fromPathString);
		getBasePath(configFromStrings, &fromStringsString);

		ASSERT_FALSE(strcmp(getDataFromString(fromPathString), getDataFromString(fromStringsString)));

		deleteWebFrameworkObject(fromPathString);
		deleteWebFrameworkObject(fromStringsString);
	}
	
	{
		const char* fromPathString;
		const char* fromStringsString;

		getRawConfiguration(configFromPath, &fromPathString);
		getRawConfiguration(configFromStrings, &fromStringsString);

		ASSERT_FALSE(strcmp(fromPathString, fromStringsString));
	}

	deleteWebFrameworkObject(configFromPath);
	deleteWebFrameworkObject(configFromStrings);
}

TEST(API, ConfigOverrideString)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;

	overrideConfigurationString(config, "webServerType", "threadPool", true);

	getConfiguration(config, &data);

	ASSERT_TRUE(strstr(getDataFromString(data), "\"webServerType\": \"threadPool\""));

	deleteWebFrameworkObject(data);
	deleteWebFrameworkObject(config);
}

TEST(API, ConfigOverrideInteger)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;

	overrideConfigurationInteger("cachingSize", 0, true);

	ASSERT_TRUE(strstr(getDataFromString(data), "\"cachingSize\": 0"));

	deleteWebFrameworkObject(data);
	deleteWebFrameworkObject(config);
}

TEST(API, ConfigOverrideBool)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;

	overrideConfigurationBoolean("usingLogging", false, true);

	ASSERT_TRUE(strstr(getDataFromString(data), "\"usingLogging\": false"));

	deleteWebFrameworkObject(data);
	deleteWebFrameworkObject(config);
}

TEST(API, ConfigOverrideStringArray)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;

	config.overrideConfiguration("loadSources", std::vector<std::string>{ "anotherSource" }, true);

	ASSERT_NE(config.getRawConfiguration(), config.getConfiguration());
}

TEST(API, ConfigOverrideIntegerArray)
{
	framework::utility::Config config = createConfig();

	config.overrideConfiguration("port", std::vector<int64_t>{ 15 }, true);

	ASSERT_NE(config.getRawConfiguration(), config.getConfiguration());
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	framework::utility::initializeWebFramework("WebFramework");

	return RUN_ALL_TESTS();
}
