#pragma once

#include "WebFrameworkException.h"

typedef void* Config;

WebFrameworkException createConfigFromPath(const char* configPath, Config* config);

WebFrameworkException createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, Config* config);

WebFrameworkException overrideConfigurationString(Config implementation, const char* key, const char* value, bool recursive);

WebFrameworkException overrideConfigurationInteger(Config implementation, const char* key, int64_t value, bool recursive);

WebFrameworkException overrideConfigurationBoolean(Config implementation, const char* key, bool value, bool recursive);

WebFrameworkException overrideConfigurationStringArray(Config implementation, const char* key, const char** value, bool recursive, int64_t size);

WebFrameworkException overrideConfigurationIntegerArray(Config implementation, const char* key, int64_t* value, bool recursive, int64_t size);

WebFrameworkException getConfiguration(Config implementation, WebFrameworkString* configuration);

WebFrameworkException getRawConfiguration(Config implementation, const char** rawConfiguration);

WebFrameworkException getBasePath(Config implementation, WebFrameworkString* basePath);

inline WebFrameworkException createConfigFromPath(const char* configPath, Config* config)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createConfigFromPath)(const char* configPath, void** exception);

	*config = CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromPath, configPath, &exception);

	return exception;
}

inline WebFrameworkException createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, Config* config)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createConfigFromString)(const char* serverConfiguration, const char* applicationDirectory, void** exception);

	*config = CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromString, serverConfiguration, applicationDirectory, &exception);

	return exception;
}

inline WebFrameworkException overrideConfigurationString(Config implementation, const char* key, const char* value, bool recursive)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationString)(void* implementation, const char* key, const char* value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationString, key, value, recursive, &exception);

	return exception;
}

inline WebFrameworkException overrideConfigurationInteger(Config implementation, const char* key, int64_t value, bool recursive)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationInteger)(void* implementation, const char* key, int64_t value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationInteger, key, value, recursive, &exception);

	return exception;
}

inline WebFrameworkException overrideConfigurationBoolean(Config implementation, const char* key, bool value, bool recursive)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationBoolean)(void* implementation, const char* key, bool value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationBoolean, key, value, recursive, &exception);

	return exception;
}

inline WebFrameworkException overrideConfigurationStringArray(Config implementation, const char* key, const char** value, bool recursive, int64_t size)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationStringArray)(void* implementation, const char* key, const char** value, bool recursive, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationStringArray, key, value, recursive, size, &exception);

	return exception;
}

inline WebFrameworkException overrideConfigurationIntegerArray(Config implementation, const char* key, int64_t* value, bool recursive, int64_t size)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationIntegerArray)(void* implementation, const char* key, int64_t* value, bool recursive, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationIntegerArray, key, value, recursive, size, &exception);

	return exception;
}

WebFrameworkException getConfiguration(Config implementation, WebFrameworkString* configuration)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getConfiguration)(void* implementation, void** exception);

	*configuration = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfiguration, &exception);

	return exception;
}

WebFrameworkException getRawConfiguration(Config implementation, const char** rawConfiguration)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getRawConfiguration)(void* implementation, void** exception);

	*rawConfiguration = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRawConfiguration, &exception);

	return exception;
}

WebFrameworkException getBasePath(Config implementation, WebFrameworkString* basePath)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getBasePath)(void* implementation, void** exception);

	*basePath = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getBasePath, &exception);

	return exception;
}
