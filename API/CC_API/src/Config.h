#pragma once

#include "WebFrameworkException.h"

typedef void* Config;

WebFrameworkException createConfigFromPath(const char* configPath, Config* config);

WebFrameworkException createConfigFromPath(const char* configPath, Config* config)
{
	WebFrameworkException exception = NULL;
	
	DLLHandler handle = getInstance(NULL);

	typedef void* (*createConfigFromPath)(const char* configPath, void** exception);

	return exception;
}
