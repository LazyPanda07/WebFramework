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

#define CALL_WEB_FRAMEWORK_FUNCTION(functionName, ...) ((functionName)findFunction(#functionName))(__VA_ARGS__)
#define CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(functionName, ...) ((functionName)findFunction(#functionName))(implementation, __VA_ARGS__)

typedef HMODULE DllHandler;
typedef void* WebFrameworkString;
typedef void* Config;
typedef void* WebFramework;
typedef void* WebFrameworkException;
typedef void* JsonBuilder;
typedef void* JsonParser;

/**
 * @brief Load WebFramework shared library
 * @param pathToDLL Path to shared library without prefixes(lib for Linux) and file extensions(.dll, .so)
 */
void wf_initialize_web_framework(const char* pathToDLL);

HMODULE wf_get_instance(const char* pathToDLL);

void* wf_find_function(const char* name);

/**
 * @brief Free memory
 * @param string String object
 */
void wf_delete_web_framework_string(WebFrameworkString string);

/**
 * @brief Free memory
 * @param config Config object
 */
void wf_delete_web_framework_config(Config config);

/**
 * @brief Free memory
 * @param webFramework WebFramework object
 */
void wf_delete_web_framework(WebFramework webFramework);

/**
 * @brief Free memory
 * @param exception Exception object
 */
void wf_delete_web_framework_exception(WebFrameworkException exception);

/**
 * @brief Free memory
 * @param builder JsonBuilder object
 */
void wf_delete_web_framework_json_builder(JsonBuilder builder);

/**
 * @brief Free memory
 * @param parser JsonParser object
 */
void wf_delete_web_framework_json_parser(JsonParser parser);

/**
 * @brief Get string from WebFrameworkString
 * @param string WebFrameworkString
 * @return String data
 */
const char* wf_get_data_from_string(WebFrameworkString string);

size_t wf_find_last_char(char* ptr, char c);

#define DEFINE_INITIALIZE_WEB_FRAMEWORK() WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void initializeWebFrameworkCC(const char* webFrameworkSharedLibraryPath) \
{ \
	initializeWebFramework(webFrameworkSharedLibraryPath); \
}
