#include "DLLHandler.h"

void wf_initialize_web_framework(const char* pathToDLL)
{
#ifdef __ANDROID__
	getInstance("libWebFramework.so");

	return;
#endif

	if (pathToDLL == NULL || !strcmp(pathToDLL, "") || !strcmp(pathToDLL, "WebFramework") ||
#ifdef __LINUX__
		!strcmp(pathToDLL, "libWebFramework.so")
#else
		!strcmp(pathToDLL, "WebFramework.dll")
#endif
		)
	{
#ifdef __LINUX__
		getInstance("libWebFramework.so");
#else
		getInstance("WebFramework.dll");
#endif

		return;
	}

#define MAX_PATH_SIZE 4096

	char fullPath[MAX_PATH_SIZE];
	char realPath[MAX_PATH_SIZE];
	const char* prefix = "";
	const char* suffix = "";

	memset(fullPath, 0, sizeof(fullPath));
	memset(realPath, 0, sizeof(realPath));

#ifdef __LINUX__
	realpath(pathToDLL, fullPath);
#else
	GetFullPathNameA(pathToDLL, sizeof(fullPath), fullPath, NULL);
#endif

#ifdef __LINUX__
	prefix = "lib";
	suffix = ".so";

	size_t index = findLastChar(fullPath, '/');
	size_t directorySize = index;
	size_t fileNameSize = strlen(fullPath + index + 1) - strlen(prefix) - strlen(suffix);
	char* directory = malloc(directorySize);
	char* fileName = malloc(fileNameSize);

	directory[directorySize] = '\0';
	fileName[fileNameSize] = '\0';

	memcpy(directory, fullPath, directorySize);
	memcpy(fileName, fullPath + index + 1 + strlen(prefix), fileNameSize);

	sprintf(realPath, "%s/%s%s%s", directory, prefix, fileName, suffix);

	free(directory);
	free(fileName);
#else
	if (strstr(fullPath, ".dll") == NULL)
	{
		suffix = ".dll";
	}

	sprintf_s(realPath, MAX_PATH_SIZE, "%s%s", fullPath, suffix);
#endif

	getInstance(realPath);

#undef MAX_PATH_SIZE
}

HMODULE wf_get_instance(const char* pathToDLL)
{
	static HMODULE instance = NULL;

	if (!instance)
	{
		if (pathToDLL)
		{
#ifdef __LINUX__
			instance = dlopen(pathToDLL, RTLD_LAZY);
#else
			instance = LoadLibraryA(pathToDLL);
#endif
			if (!instance)
			{
#ifdef __LINUX__
				printf("Can't load %s or its dependencies\n", pathToDLL);
#else
				printf("GetLastError(): %llu\n", (uint64_t)GetLastError());
#endif
				exit(-1);
			}

		}
		else
		{
			printf("WebFramework must be initialized with initializeWebFramework function\n");

			exit(-1);
		}
	}

	return instance;
}

void* wf_find_function(const char* name)
{
	HMODULE instance = getInstance(NULL);

#ifdef __LINUX__
	return dlsym(instance, name);
#else
	return (void*)GetProcAddress(instance, name);
#endif
}

void wf_delete_web_framework_string(WebFrameworkString string)
{
	typedef void (*deleteWebFrameworkString)(void* string);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkString, string);
}

void wf_delete_web_framework_config(Config config)
{
	typedef void (*deleteWebFrameworkConfig)(void* config);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkConfig, config);
}

void wf_delete_web_framework(WebFramework webFramework)
{
	typedef void (*deleteWebFramework)(void* webFramework);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFramework, webFramework);
}

void wf_delete_web_framework_exception(WebFrameworkException exception)
{
	typedef void (*deleteWebFrameworkException)(void* exception);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkException, exception);
}

void wf_delete_web_framework_json_builder(JsonBuilder builder)
{
	typedef void (*deleteWebFrameworkJsonBuilder)(void* builder);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonBuilder, builder);
}

void wf_delete_web_framework_json_parser(JsonParser parser)
{
	typedef void (*deleteWebFrameworkJsonParser)(void* parser);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonParser, parser);
}

const char* wf_get_data_from_string(WebFrameworkString string)
{
	typedef const char* (*getDataFromString)(void* implementation);

	return CALL_WEB_FRAMEWORK_FUNCTION(getDataFromString, string);
}

size_t wf_find_last_char(char* ptr, char c)
{
	size_t result = -1;

	for (size_t i = 0;; i++)
	{
		if (ptr[i] == 0)
		{
			return result;
		}

		if (ptr[i] == c)
		{
			result = i;
		}
	}

	return result;
}
