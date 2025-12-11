#pragma once

#include "WebFrameworkException.h"

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
 * @brief Get current config Json string data
 * @param implementation Config
 * @param configuration Result string
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException getConfiguration(Config implementation, WebFrameworkString* configuration);

/**
 * @brief Get raw config Json string data
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
