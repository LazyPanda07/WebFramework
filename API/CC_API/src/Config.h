#pragma once

#include "WebFrameworkException.h"

typedef void* Config;

/**
 * @brief Create Config
 * @param configPath Path to *.json config file
 * @param config Result Config
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException createConfigFromPath(const char* configPath, Config* config);

/**
 * @brief Create Config
 * @param serverConfiguration *.json config file content
 * @param applicationDirectory Working directory
 * @param config Result Config
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, Config* config);

/**
 * @brief Override Config string
 * @param implementation Config
 * @param key JSON key
 * @param value New string value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException overrideConfigurationString(Config implementation, const char* key, const char* value, bool recursive);

/**
 * @brief Override Config integer
 * @param implementation Config
 * @param key JSON key
 * @param value New integer value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException overrideConfigurationInteger(Config implementation, const char* key, int64_t value, bool recursive);

/**
 * @brief Override Config bool
 * @param implementation Config
 * @param key JSON key
 * @param value New bool value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException overrideConfigurationBoolean(Config implementation, const char* key, bool value, bool recursive);

/**
 * @brief Override Config string array
 * @param implementation Config
 * @param key JSON key
 * @param value New string array value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException overrideConfigurationStringArray(Config implementation, const char* key, const char** value, bool recursive, int64_t size);

/**
 * @brief Override Config integer array
 * @param implementation Config
 * @param key JSON key
 * @param value New integer array value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException overrideConfigurationIntegerArray(Config implementation, const char* key, int64_t* value, bool recursive, int64_t size);

/**
 * @brief Override config file directory
 * @param implementation Config
 * @param basePath New base path
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException overrideBasePath(Config implementation, const char* basePath);

/**
 * @brief Get string from config
 * @param implementation Config
 * @param key Config key
 * @param recursive Search recursively
 * @param result Config string value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException getConfigurationString(Config implementation, const char* key, bool recursive, WebFrameworkString* result);

/**
 * @brief Get integer from config
 * @param implementation Config
 * @param key Config key
 * @param recursive Search recursively
 * @param result Config integer value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException getConfigurationInteger(Config implementation, const char* key, bool recursive, int64_t* result);

/**
 * @brief Get boolean from config
 * @param implementation Config
 * @param key Config key
 * @param recursive Search recursively
 * @param result Config boolean value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException getConfigurationBoolean(Config implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get current config JSON string data
 * @param implementation Config
 * @param configuration Result string
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException getConfiguration(Config implementation, WebFrameworkString* configuration);

/**
 * @brief Get raw config JSON string data
 * @param implementation
 * @param rawConfiguration
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException getRawConfiguration(Config implementation, const char** rawConfiguration);

/**
 * @brief Get config file directory
 * @param implementation Config
 * @param basePath Result string
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
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

inline WebFrameworkException overrideBasePath(Config implementation, const char* basePath)
{
	WebFrameworkException exception = NULL;

	typedef void (*overrideBasePath)(void* implementation, const char* basePath, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideBasePath, basePath, &exception);

	return exception;
}

inline WebFrameworkException getConfigurationString(Config implementation, const char* key, bool recursive, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getConfigurationString)(void* implementation, const char* key, bool recursive, WebFrameworkException* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationString, key, recursive, exception);

	return exception;
}

inline WebFrameworkException getConfigurationInteger(Config implementation, const char* key, bool recursive, int64_t* result)
{
	WebFrameworkException exception = NULL;

	typedef int64_t(*getConfigurationInteger)(void* implementation, const char* key, bool recursive, WebFrameworkException* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationInteger, key, recursive, exception);

	return exception;
}

inline WebFrameworkException getConfigurationBoolean(Config implementation, const char* key, bool recursive, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool(*getConfigurationBoolean)(void* implementation, const char* key, bool recursive, WebFrameworkException* exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationBoolean, key, recursive, exception);

	return exception;
}

inline WebFrameworkException getConfiguration(Config implementation, WebFrameworkString* configuration)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getConfiguration)(void* implementation, void** exception);

	*configuration = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfiguration, &exception);

	return exception;
}

inline WebFrameworkException getRawConfiguration(Config implementation, const char** rawConfiguration)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getRawConfiguration)(void* implementation, void** exception);

	*rawConfiguration = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRawConfiguration, &exception);

	return exception;
}

inline WebFrameworkException getBasePath(Config implementation, WebFrameworkString* basePath)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getBasePath)(void* implementation, void** exception);

	*basePath = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getBasePath, &exception);

	return exception;
}
