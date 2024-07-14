#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#ifdef __LINUX__
#include <dlfcn.h>

typedef void* HMODULE;
#else
#include <Windows.h>
#endif

typedef HMODULE DLLHandler;

void initializeWebFramework(const char* pathToDLL);

void* getInstance(const char* pathToDLL);

inline void initializeWebFramework(const char* pathToDLL)
{
	char fullPath[4096];

	memset(fullPath, 0, sizeof(fullPath));
	
#ifdef __LINUX__
	realpath(pathToDLL, fullPath);
#else
	GetFullPathNameA(pathToDLL, sizeof(fullPath), fullPath, NULL);
#endif
	
	getInstance(fullPath);
}

inline void* getInstance(const char* pathToDLL)
{
	static HMODULE* instance = NULL;

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
				printf("Can't load %s or its dependencies", pathToDLL);
#else
				printf("GetLastError(): %zu", GetLastError());
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
