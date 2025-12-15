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

#define CALL_WEB_FRAMEWORK_FUNCTION(functionName, ...) ((functionName)wf_find_function(#functionName))(__VA_ARGS__)
#define CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(functionName, ...) ((functionName)wf_find_function(#functionName))(implementation, __VA_ARGS__)

typedef HMODULE dll_handler_t;
typedef void* web_framework_string_t;
typedef void* config_t;
typedef void* web_framework_t;
typedef void* web_framework_exception_t;
typedef void* json_builder_t;
typedef void* json_parser_t;

/**
 * @brief Load web_framework_t shared library
 * @param pathToDLL Path to shared library without prefixes(lib for Linux) and file extensions(.dll, .so)
 */
void wf_initialize_web_framework(const char* path_to_dll);

HMODULE wf_get_instance(const char* path_to_dll);

void* wf_find_function(const char* name);

/**
 * @brief Free memory
 * @param string String object
 */
void wf_delete_string(web_framework_string_t string);

/**
 * @brief Free memory
 * @param config config_t object
 */
void wf_delete_config(config_t config);

/**
 * @brief Free memory
 * @param webFramework web_framework_t object
 */
void wf_delete_web_framework(web_framework_t web_framework);

/**
 * @brief Free memory
 * @param exception Exception object
 */
void wf_delete_web_framework_exception(web_framework_exception_t exception);

/**
 * @brief Free memory
 * @param builder json_builder_t object
 */
void wf_delete_json_builder(json_builder_t builder);

/**
 * @brief Free memory
 * @param parser json_parser_t object
 */
void wf_delete_json_parser(json_parser_t parser);

/**
 * @brief Get string from web_framework_string_t
 * @param string web_framework_string_t
 * @return String data
 */
const char* wf_get_data_from_string(web_framework_string_t string);

#define DEFINE_INITIALIZE_WEB_FRAMEWORK() WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void initializeWebFrameworkCC(const char* webFrameworkSharedLibraryPath) \
{ \
	wf_initialize_web_framework(webFrameworkSharedLibraryPath); \
}
