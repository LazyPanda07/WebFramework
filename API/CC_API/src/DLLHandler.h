#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __LINUX__
#include <dlfcn.h>

typedef void* HMODULE;
#else
#include <Windows.h>
#endif

#define CALL_WEB_FRAMEWORK_FUNCTION(functionName, ...) ((functionName)findFunction(#functionName))(__VA_ARGS__)
#define CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(functionName, ...) ((functionName)findFunction(#functionName))(implementation, __VA_ARGS__)

typedef HMODULE DLLHandler;
typedef void* WebFrameworkString;
typedef void* Config;
typedef void* WebFramework;
typedef void* WebFrameworkException;

/**
 * @brief Load WebFramework shared library
 * @param pathToDLL Path to shared library without prefixes(lib for Linux) and file extensions(.dll, .so)
 */
void initializeWebFramework(const char* pathToDLL);

HMODULE getInstance(const char* pathToDLL);

void* findFunction(const char* name);

/**
 * @brief Free memory
 * @param string String object
 */
void deleteWebFrameworkString(WebFrameworkString string);

/**
 * @brief Free memory
 * @param config Config object
 */
void deleteWebFrameworkConfig(Config config);

/**
 * @brief Free memory
 * @param webFramework WebFramework object
 */
void deleteWebFramework(WebFramework webFramework);

/**
 * @brief Free memory
 * @param exception Exception object
 */
void deleteWebFrameworkException(WebFrameworkException exception);

/**
 * @brief Get string from WebFrameworkString
 * @param string WebFrameworkString
 * @return String data
 */
const char* getDataFromString(WebFrameworkString string);

size_t findLastChar(char* ptr, char c);

inline void initializeWebFramework(const char* pathToDLL)
{
	if (pathToDLL == NULL)
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

	memset(fullPath, 0, sizeof(fullPath));
	memset(realPath, 0, sizeof(realPath));
	
#ifdef __LINUX__
	realpath(pathToDLL, fullPath);
#else
	GetFullPathNameA(pathToDLL, sizeof(fullPath), fullPath, NULL);
#endif

#ifdef __LINUX__
	size_t index = findLastChar(fullPath, '/');
	size_t directorySize = index;
	size_t fileNameSize = strlen(fullPath + index + 1);
	char* directory = malloc(directorySize);
	char* fileName = malloc(fileNameSize);

	directory[directorySize] = 0;
	fileName[fileNameSize] = 0;

	memcpy(directory, fullPath, directorySize);
	memcpy(fileName, fullPath + index + 1, fileNameSize);

	sprintf(realPath, "%s/lib%s.so", directory, fileName);

	free(directory);
	free(fileName);
#else
	sprintf_s(realPath, MAX_PATH_SIZE, "%s.dll", fullPath);
#endif
	
	getInstance(realPath);

#undef MAX_PATH_SIZE
}

inline HMODULE getInstance(const char* pathToDLL)
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

inline void* findFunction(const char* name)
{
	HMODULE instance = getInstance(NULL);

#ifdef __LINUX__
	return dlsym(instance, name);
#else
	return (void*)GetProcAddress(instance, name);
#endif
}

inline void deleteWebFrameworkString(WebFrameworkString string)
{
	typedef void (*deleteWebFrameworkString)(void* string);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkString, string);
}

inline void deleteWebFrameworkConfig(Config config)
{
	typedef void (*deleteWebFrameworkConfig)(void* config);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkConfig, config);
}

inline void deleteWebFramework(WebFramework webFramework)
{
	typedef void (*deleteWebFramework)(void* webFramework);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFramework, webFramework);
}

inline void deleteWebFrameworkException(WebFrameworkException exception)
{
	typedef void (*deleteWebFrameworkException)(void* exception);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkException, exception);
}

inline const char* getDataFromString(WebFrameworkString string)
{
	typedef const char* (*getDataFromString)(void* implementation);

	return CALL_WEB_FRAMEWORK_FUNCTION(getDataFromString, string);
}

inline size_t findLastChar(char* ptr, char c)
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
