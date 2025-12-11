#pragma once

#include "WebFrameworkException.h"

/**
 * @brief Create Config
 * @param configPath Path to *.json config file
 * @param config Result Config
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_create_config_from_path(const char* config_path, Config* config);

/**
 * @brief Create Config
 * @param serverConfiguration *.json config file content
 * @param applicationDirectory Working directory
 * @param config Result Config
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_create_config_from_string(const char* server_configuration, const char* application_directory, Config* config);

/**
 * @brief Override Config string
 * @param implementation Config
 * @param key JSON key
 * @param value New string value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_override_configuration_string(Config implementation, const char* key, const char* value, bool recursive);

/**
 * @brief Override Config integer
 * @param implementation Config
 * @param key JSON key
 * @param value New integer value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_override_configuration_integer(Config implementation, const char* key, int64_t value, bool recursive);

/**
 * @brief Override Config bool
 * @param implementation Config
 * @param key JSON key
 * @param value New bool value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_override_configuration_boolean(Config implementation, const char* key, bool value, bool recursive);

/**
 * @brief Override Config string array
 * @param implementation Config
 * @param key JSON key
 * @param value New string array value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_override_configuration_string_array(Config implementation, const char* key, const char** value, bool recursive, int64_t size);

/**
 * @brief Override Config integer array
 * @param implementation Config
 * @param key JSON key
 * @param value New integer array value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_override_configuration_integer_array(Config implementation, const char* key, int64_t* value, bool recursive, int64_t size);

/**
 * @brief Override config file directory
 * @param implementation Config
 * @param basePath New base path
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_override_base_path(Config implementation, const char* basePath);

/**
 * @brief Get string from config
 * @param implementation Config
 * @param key Config key
 * @param recursive Search recursively
 * @param result Config string value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_get_configuration_string(Config implementation, const char* key, bool recursive, WebFrameworkString* result);

/**
 * @brief Get integer from config
 * @param implementation Config
 * @param key Config key
 * @param recursive Search recursively
 * @param result Config integer value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_get_configuration_integer(Config implementation, const char* key, bool recursive, int64_t* result);

/**
 * @brief Get boolean from config
 * @param implementation Config
 * @param key Config key
 * @param recursive Search recursively
 * @param result Config boolean value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_get_configuration_boolean(Config implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get current config Json string data
 * @param implementation Config
 * @param configuration Result string
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_get_configuration(Config implementation, WebFrameworkString* configuration);

/**
 * @brief Get raw config Json string data
 * @param implementation
 * @param rawConfiguration
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_get_raw_configuration(Config implementation, const char** rawConfiguration);

/**
 * @brief Get config file directory
 * @param implementation Config
 * @param basePath Result string
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException wf_get_base_path(Config implementation, WebFrameworkString* basePath);
