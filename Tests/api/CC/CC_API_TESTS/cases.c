#include "cases.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <import.h>
#include <json_object.h>

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

static config_t createConfig()
{
	config_t result;

	wf_create_config_from_path("multi_threaded_config.json", &result);

	return result;
}

void initialize()
{
	wf_initialize_web_framework("WebFramework");
}

void configConstructors(bool* configuration, bool* basePath, bool* rawConfiguration)
{
	config_t configFromPath = createConfig();
	config_t configFromStrings;
	char* data = getConfigurationData();
	char currentDirectory[4096];

	memset(currentDirectory, 0, sizeof(currentDirectory));

#ifdef __LINUX__
	getcwd(currentDirectory, sizeof(currentDirectory));
#else
	GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory);
#endif

	wf_create_config_from_string(data, currentDirectory, &configFromStrings);

	free(data);

	{
		web_framework_string_t fromPathString;
		web_framework_string_t fromStringsString;

		wf_get_configuration(configFromPath, &fromPathString);
		wf_get_configuration(configFromStrings, &fromStringsString);

		*configuration = strcmp(wf_get_data_from_string(fromPathString), wf_get_data_from_string(fromStringsString));

		wf_delete_web_framework_string(fromPathString);
		wf_delete_web_framework_string(fromStringsString);
	}

	{
		web_framework_string_t fromPathString;
		web_framework_string_t fromStringsString;

		wf_get_base_path(configFromPath, &fromPathString);
		wf_get_base_path(configFromStrings, &fromStringsString);

		*basePath = strcmp(wf_get_data_from_string(fromPathString), wf_get_data_from_string(fromStringsString));

		wf_delete_web_framework_string(fromPathString);
		wf_delete_web_framework_string(fromStringsString);
	}

	{
		const char* fromPathString;
		const char* fromStringsString;

		wf_get_raw_configuration(configFromPath, &fromPathString);
		wf_get_raw_configuration(configFromStrings, &fromStringsString);

		*rawConfiguration = strcmp(fromPathString, fromStringsString);
	}

	wf_delete_web_framework_config(configFromPath);
	wf_delete_web_framework_config(configFromStrings);
}

void configOverrideString(bool* assertTrue)
{
	config_t config = createConfig();
	web_framework_string_t data = NULL;

	wf_override_configuration_string(config, "webServerType", "threadPool", true);

	wf_get_configuration(config, &data);

	*assertTrue = strstr(wf_get_data_from_string(data), "\"webServerType\": \"threadPool\"");

	wf_delete_web_framework_string(data);
	wf_delete_web_framework_config(config);
}

void configOverrideInteger(bool* assertTrue)
{
	config_t config = createConfig();
	web_framework_string_t data = NULL;

	wf_override_configuration_integer(config, "cachingSize", 0, true);

	wf_get_configuration(config, &data);

	*assertTrue = strstr(wf_get_data_from_string(data), "\"cachingSize\": 0");

	wf_delete_web_framework_string(data);
	wf_delete_web_framework_config(config);
}

void configOverrideBool(bool* assertTrue)
{
	config_t config = createConfig();
	web_framework_string_t data = NULL;

	wf_override_configuration_boolean(config, "usingLogging", false, true);

	wf_get_configuration(config, &data);

	*assertTrue = strstr(wf_get_data_from_string(data), "\"usingLogging\": false");

	wf_delete_web_framework_string(data);
	wf_delete_web_framework_config(config);
}

void configOverrideStringArray(bool* assertFalse)
{
	config_t config = createConfig();
	web_framework_string_t data = NULL;
	web_framework_string_t rawData = NULL;
	const char** values = malloc(sizeof(const char*));

	values[0] = "anotherSource";

	wf_override_configuration_string_array(config, "loadSources", values, true, 1);

	wf_get_configuration(config, &data);
	wf_get_configuration(config, &rawData);

	*assertFalse = strcmp(wf_get_data_from_string(data), wf_get_data_from_string(rawData));

	wf_delete_web_framework_string(data);
	wf_delete_web_framework_string(rawData);
	wf_delete_web_framework_config(config);

	free(values);
}

void configOverrideIntegerArray(bool* assertFalse)
{
	config_t config = createConfig();
	web_framework_string_t data = NULL;
	web_framework_string_t rawData = NULL;
	int64_t* values = malloc(sizeof(int64_t));

	values[0] = 15;

	wf_override_configuration_integer_array(config, "port", values, true, 1);

	wf_get_configuration(config, &data);
	wf_get_configuration(config, &rawData);

	*assertFalse = strcmp(wf_get_data_from_string(data), wf_get_data_from_string(rawData));

	wf_delete_web_framework_string(data);
	wf_delete_web_framework_string(rawData);
	wf_delete_web_framework_config(config);

	free(values);
}
