#include "cases.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "import.h"

#ifdef __LINUX__
#include <unistd.h>
#else
#include <Windows.h>

#pragma warning(disable: 6011)
#pragma warning(disable: 6386)
#endif

static char* getConfigurationData()
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

static Config createConfig()
{
	Config result;

	createConfigFromPath("multi_threaded_config.json", &result);

	return result;
}

void initialize()
{
	initializeWebFramework("WebFramework");
}

void configConstructors(bool* configuration, bool* basePath, bool* rawConfiguration)
{
	Config configFromPath = createConfig();
	Config configFromStrings;
	char* data = getConfigurationData();
	char currentDirectory[4096];

	memset(currentDirectory, 0, sizeof(currentDirectory));

#ifdef __LINUX__
	getcwd(currentDirectory, sizeof(currentDirectory));
#else
	GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory);
#endif

	createConfigFromString(data, currentDirectory, &configFromStrings);

	free(data);

	{
		WebFrameworkString fromPathString;
		WebFrameworkString fromStringsString;

		getConfiguration(configFromPath, &fromPathString);
		getConfiguration(configFromStrings, &fromStringsString);

		*configuration = strcmp(getDataFromString(fromPathString), getDataFromString(fromStringsString));

		deleteWebFrameworkString(fromPathString);
		deleteWebFrameworkString(fromStringsString);
	}

	{
		WebFrameworkString fromPathString;
		WebFrameworkString fromStringsString;

		getBasePath(configFromPath, &fromPathString);
		getBasePath(configFromStrings, &fromStringsString);

		*basePath = strcmp(getDataFromString(fromPathString), getDataFromString(fromStringsString));

		deleteWebFrameworkString(fromPathString);
		deleteWebFrameworkString(fromStringsString);
	}

	{
		const char* fromPathString;
		const char* fromStringsString;

		getRawConfiguration(configFromPath, &fromPathString);
		getRawConfiguration(configFromStrings, &fromStringsString);

		*rawConfiguration = strcmp(fromPathString, fromStringsString);
	}

	deleteWebFrameworkConfig(configFromPath);
	deleteWebFrameworkConfig(configFromStrings);
}

void configOverrideString(bool* assertTrue)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;

	overrideConfigurationString(config, "webServerType", "threadPool", true);

	getConfiguration(config, &data);

	*assertTrue = strstr(getDataFromString(data), "\"webServerType\": \"threadPool\"");

	deleteWebFrameworkString(data);
	deleteWebFrameworkConfig(config);
}

void configOverrideInteger(bool* assertTrue)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;

	overrideConfigurationInteger(config, "cachingSize", 0, true);

	getConfiguration(config, &data);

	*assertTrue = strstr(getDataFromString(data), "\"cachingSize\": 0");

	deleteWebFrameworkString(data);
	deleteWebFrameworkConfig(config);
}

void configOverrideBool(bool* assertTrue)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;

	overrideConfigurationBoolean(config, "usingLogging", false, true);

	getConfiguration(config, &data);

	*assertTrue = strstr(getDataFromString(data), "\"usingLogging\": false");

	deleteWebFrameworkString(data);
	deleteWebFrameworkConfig(config);
}

void configOverrideStringArray(bool* assertFalse)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;
	WebFrameworkString rawData = NULL;
	const char** values = malloc(sizeof(const char*));

	values[0] = "anotherSource";

	overrideConfigurationStringArray(config, "loadSources", values, true, 1);

	getConfiguration(config, &data);
	getConfiguration(config, &rawData);

	*assertFalse = strcmp(getDataFromString(data), getDataFromString(rawData));

	deleteWebFrameworkString(data);
	deleteWebFrameworkString(rawData);
	deleteWebFrameworkConfig(config);

	free(values);
}

void configOverrideIntegerArray(bool* assertFalse)
{
	Config config = createConfig();
	WebFrameworkString data = NULL;
	WebFrameworkString rawData = NULL;
	int64_t* values = malloc(sizeof(int64_t));

	values[0] = 15;

	overrideConfigurationIntegerArray(config, "port", values, true, 1);

	getConfiguration(config, &data);
	getConfiguration(config, &rawData);

	*assertFalse = strcmp(getDataFromString(data), getDataFromString(rawData));

	deleteWebFrameworkString(data);
	deleteWebFrameworkString(rawData);
	deleteWebFrameworkConfig(config);

	free(values);
}
