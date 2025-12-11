#include "config.h"

web_framework_exception_t wf_create_config_from_path(const char* configPath, config_t* config)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createConfigFromPath)(const char* configPath, void** exception);

	*config = CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromPath, configPath, &exception);

	return exception;
}

web_framework_exception_t wf_create_config_from_string(const char* serverConfiguration, const char* applicationDirectory, config_t* config)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createConfigFromString)(const char* serverConfiguration, const char* applicationDirectory, void** exception);

	*config = CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromString, serverConfiguration, applicationDirectory, &exception);

	return exception;
}

web_framework_exception_t wf_override_configuration_string(config_t implementation, const char* key, const char* value, bool recursive)
{
	web_framework_exception_t exception = NULL;

	typedef void (*overrideConfigurationString)(void* implementation, const char* key, const char* value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationString, key, value, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_override_configuration_integer(config_t implementation, const char* key, int64_t value, bool recursive)
{
	web_framework_exception_t exception = NULL;

	typedef void (*overrideConfigurationInteger)(void* implementation, const char* key, int64_t value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationInteger, key, value, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_override_configuration_boolean(config_t implementation, const char* key, bool value, bool recursive)
{
	web_framework_exception_t exception = NULL;

	typedef void (*overrideConfigurationBoolean)(void* implementation, const char* key, bool value, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationBoolean, key, value, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_override_configuration_string_array(config_t implementation, const char* key, const char** value, bool recursive, int64_t size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*overrideConfigurationStringArray)(void* implementation, const char* key, const char** value, bool recursive, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationStringArray, key, value, recursive, size, &exception);

	return exception;
}

web_framework_exception_t wf_override_configuration_integer_array(config_t implementation, const char* key, int64_t* value, bool recursive, int64_t size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*overrideConfigurationIntegerArray)(void* implementation, const char* key, int64_t* value, bool recursive, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationIntegerArray, key, value, recursive, size, &exception);

	return exception;
}

web_framework_exception_t wf_override_base_path(config_t implementation, const char* basePath)
{
	web_framework_exception_t exception = NULL;

	typedef void (*overrideBasePath)(void* implementation, const char* basePath, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideBasePath, basePath, &exception);

	return exception;
}

web_framework_exception_t wf_get_configuration_string(config_t implementation, const char* key, bool recursive, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getConfigurationString)(void* implementation, const char* key, bool recursive, web_framework_exception_t* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationString, key, recursive, exception);

	return exception;
}

web_framework_exception_t wf_get_configuration_integer(config_t implementation, const char* key, bool recursive, int64_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int64_t(*getConfigurationInteger)(void* implementation, const char* key, bool recursive, web_framework_exception_t* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationInteger, key, recursive, exception);

	return exception;
}

web_framework_exception_t wf_get_configuration_boolean(config_t implementation, const char* key, bool recursive, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool(*getConfigurationBoolean)(void* implementation, const char* key, bool recursive, web_framework_exception_t* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationBoolean, key, recursive, exception);

	return exception;
}

web_framework_exception_t wf_get_configuration(config_t implementation, web_framework_string_t* configuration)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getConfiguration)(void* implementation, void** exception);

	*configuration = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfiguration, &exception);

	return exception;
}

web_framework_exception_t wf_get_raw_configuration(config_t implementation, const char** raw_configuration)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getRawConfiguration)(void* implementation, void** exception);

	*raw_configuration = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRawConfiguration, &exception);

	return exception;
}

web_framework_exception_t wf_get_base_path(config_t implementation, web_framework_string_t* base_path)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getBasePath)(void* implementation, void** exception);

	*base_path = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getBasePath, &exception);

	return exception;
}
