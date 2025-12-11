#pragma once

#include "web_framework_exception.h"

/**
 * @brief Create config_t
 * @param configPath Path to *.json config file
 * @param config Result config_t
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_create_config_from_path(const char* config_path, config_t* config);

/**
 * @brief Create config_t
 * @param serverConfiguration *.json config file content
 * @param applicationDirectory Working directory
 * @param config Result config_t
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_create_config_from_string(const char* server_configuration, const char* application_directory, config_t* config);

/**
 * @brief Override config_t string
 * @param implementation config_t
 * @param key JSON key
 * @param value New string value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_override_configuration_string(config_t implementation, const char* key, const char* value, bool recursive);

/**
 * @brief Override config_t integer
 * @param implementation config_t
 * @param key JSON key
 * @param value New integer value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_override_configuration_integer(config_t implementation, const char* key, int64_t value, bool recursive);

/**
 * @brief Override config_t bool
 * @param implementation config_t
 * @param key JSON key
 * @param value New bool value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_override_configuration_boolean(config_t implementation, const char* key, bool value, bool recursive);

/**
 * @brief Override config_t string array
 * @param implementation config_t
 * @param key JSON key
 * @param value New string array value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_override_configuration_string_array(config_t implementation, const char* key, const char** value, bool recursive, int64_t size);

/**
 * @brief Override config_t integer array
 * @param implementation config_t
 * @param key JSON key
 * @param value New integer array value
 * @param recursive Recursive search for key
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_override_configuration_integer_array(config_t implementation, const char* key, int64_t* value, bool recursive, int64_t size);

/**
 * @brief Override config file directory
 * @param implementation config_t
 * @param basePath New base path
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_override_base_path(config_t implementation, const char* basePath);

/**
 * @brief Get string from config
 * @param implementation config_t
 * @param key config_t key
 * @param recursive Search recursively
 * @param result config_t string value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_get_configuration_string(config_t implementation, const char* key, bool recursive, web_framework_string_t* result);

/**
 * @brief Get integer from config
 * @param implementation config_t
 * @param key config_t key
 * @param recursive Search recursively
 * @param result config_t integer value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_get_configuration_integer(config_t implementation, const char* key, bool recursive, int64_t* result);

/**
 * @brief Get boolean from config
 * @param implementation config_t
 * @param key config_t key
 * @param recursive Search recursively
 * @param result config_t boolean value
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_get_configuration_boolean(config_t implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get current config Json string data
 * @param implementation config_t
 * @param configuration Result string
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_get_configuration(config_t implementation, web_framework_string_t* configuration);

/**
 * @brief Get raw config Json string data
 * @param implementation
 * @param rawConfiguration
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_get_raw_configuration(config_t implementation, const char** raw_configuration);

/**
 * @brief Get config file directory
 * @param implementation config_t
 * @param basePath Result string
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t wf_get_base_path(config_t implementation, web_framework_string_t* base_path);
