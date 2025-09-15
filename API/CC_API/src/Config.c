#include "Config.h"

WebFrameworkException createConfigFromPath(const char* configPath, Config* config)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createConfigFromPath)(const char* configPath, void** exception);

	*config = CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromPath, configPath, &exception);

	return exception;
}

WebFrameworkException createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, Config* config)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createConfigFromString)(const char* serverConfiguration, const char* applicationDirectory, void** exception);

	*config = CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromString, serverConfiguration, applicationDirectory, &exception);

	return exception;
}

WebFrameworkException overrideConfigurationString(Config implementation, const char* key, const char* value, bool recursive)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationString)(void* implementation, const char* key, const char* value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationString, key, value, recursive, &exception);

	return exception;
}

WebFrameworkException overrideConfigurationInteger(Config implementation, const char* key, int64_t value, bool recursive)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationInteger)(void* implementation, const char* key, int64_t value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationInteger, key, value, recursive, &exception);

	return exception;
}

WebFrameworkException overrideConfigurationBoolean(Config implementation, const char* key, bool value, bool recursive)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationBoolean)(void* implementation, const char* key, bool value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationBoolean, key, value, recursive, &exception);

	return exception;
}

WebFrameworkException overrideConfigurationStringArray(Config implementation, const char* key, const char** value, bool recursive, int64_t size)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationStringArray)(void* implementation, const char* key, const char** value, bool recursive, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationStringArray, key, value, recursive, size, &exception);

	return exception;
}

WebFrameworkException overrideConfigurationIntegerArray(Config implementation, const char* key, int64_t* value, bool recursive, int64_t size)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideConfigurationIntegerArray)(void* implementation, const char* key, int64_t* value, bool recursive, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationIntegerArray, key, value, recursive, size, &exception);

	return exception;
}

WebFrameworkException overrideBasePath(Config implementation, const char* basePath)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideBasePath)(void* implementation, const char* basePath, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideBasePath, basePath, &exception);

	return exception;
}

WebFrameworkException getConfigurationString(Config implementation, const char* key, bool recursive, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getConfigurationString)(void* implementation, const char* key, bool recursive, WebFrameworkException* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationString, key, recursive, exception);

	return exception;
}

WebFrameworkException getConfigurationInteger(Config implementation, const char* key, bool recursive, int64_t* result)
{
	WebFrameworkException exception = NULL;

	typedef int64_t(*getConfigurationInteger)(void* implementation, const char* key, bool recursive, WebFrameworkException* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationInteger, key, recursive, exception);

	return exception;
}

WebFrameworkException getConfigurationBoolean(Config implementation, const char* key, bool recursive, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool(*getConfigurationBoolean)(void* implementation, const char* key, bool recursive, WebFrameworkException* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationBoolean, key, recursive, exception);

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