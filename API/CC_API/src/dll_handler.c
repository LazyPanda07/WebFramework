#include "dll_handler.h"

static size_t findLastChar(char* ptr, char c);

void wf_initialize_web_framework(const char* path_to_dll)
{
#ifdef __ANDROID__
	wf_get_instance("libWebFramework.so");

	return;
#endif

	if (path_to_dll == NULL || !strcmp(path_to_dll, "") || !strcmp(path_to_dll, "WebFramework") ||
#ifdef __LINUX__
		!strcmp(path_to_dll, "libWebFramework.so")
#else
		!strcmp(path_to_dll, "WebFramework.dll")
#endif
		)
	{
#ifdef __LINUX__
		wf_get_instance("libWebFramework.so");
#else
		wf_get_instance("WebFramework.dll");
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
	realpath(path_to_dll, fullPath);
#else
	GetFullPathNameA(path_to_dll, sizeof(fullPath), fullPath, NULL);
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

	wf_get_instance(realPath);

#undef MAX_PATH_SIZE
}

HMODULE wf_get_instance(const char* path_to_dll)
{
	static HMODULE instance = NULL;

	if (!instance)
	{
		if (path_to_dll)
		{
#ifdef __LINUX__
			instance = dlopen(path_to_dll, RTLD_LAZY);
#else
			instance = LoadLibraryA(path_to_dll);
#endif
			if (!instance)
			{
#ifdef __LINUX__
				printf("Can't load %s or its dependencies\n", path_to_dll);
#else
				printf("GetLastError(): %llu\n", (uint64_t)GetLastError());
#endif
				exit(-1);
			}

		}
		else
		{
			printf("web_framework_t must be initialized with initializeWebFramework function\n");

			exit(-1);
		}
	}

	return instance;
}

void* wf_find_function(const char* name)
{
	HMODULE instance = wf_get_instance(NULL);

#ifdef __LINUX__
	return dlsym(instance, name);
#else
	return (void*)GetProcAddress(instance, name);
#endif
}

void wf_delete_web_framework_string(web_framework_string_t string)
{
	typedef void (*deleteWebFrameworkString)(void* string);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkString, string);
}

void wf_delete_web_framework_config(config_t config)
{
	typedef void (*deleteWebFrameworkConfig)(void* config);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkConfig, config);
}

void wf_delete_web_framework(web_framework_t webFramework)
{
	typedef void (*deleteWebFramework)(void* webFramework);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFramework, webFramework);
}

void wf_delete_web_framework_exception(web_framework_exception_t exception)
{
	typedef void (*deleteWebFrameworkException)(void* exception);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkException, exception);
}

void wf_delete_web_framework_json_builder(json_builder_t builder)
{
	typedef void (*deleteWebFrameworkJsonBuilder)(void* builder);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonBuilder, builder);
}

void wf_delete_web_framework_json_parser(json_parser_t parser)
{
	typedef void (*deleteWebFrameworkJsonParser)(void* parser);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonParser, parser);
}

const char* wf_get_data_from_string(web_framework_string_t string)
{
	typedef const char* (*getDataFromString)(void* implementation);

	return CALL_WEB_FRAMEWORK_FUNCTION(getDataFromString, string);
}

size_t findLastChar(char* ptr, char c)
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
