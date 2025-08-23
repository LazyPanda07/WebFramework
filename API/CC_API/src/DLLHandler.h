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

typedef HMODULE DLLHandler;
typedef void* WebFrameworkString;
typedef void* Config;
typedef void* WebFramework;
typedef void* WebFrameworkException;
typedef void* JSONBuilder;
typedef void* JSONParser;

/**
 * @brief Load WebFramework shared library
 * @param pathToDLL Path to shared library without prefixes(lib for Linux) and file extensions(.dll, .so)
 */
void initializeWebFramework(const char* pathToDLL);

HMODULE getInstance(const char* pathToDLL);

void* findFunction(const char* name);

/**
 * @brief Free memory
 * @param string String object
 */
void deleteWebFrameworkString(WebFrameworkString string);

/**
 * @brief Free memory
 * @param config Config object
 */
void deleteWebFrameworkConfig(Config config);

/**
 * @brief Free memory
 * @param webFramework WebFramework object
 */
void deleteWebFramework(WebFramework webFramework);

/**
 * @brief Free memory
 * @param exception Exception object
 */
void deleteWebFrameworkException(WebFrameworkException exception);

/**
 * @brief Free memory
 * @param builder JSONBuilder object
 */
void deleteWebFrameworkJSONBuider(JSONBuilder builder);

/**
 * @brief Free memory
 * @param parser JSONParser object
 */
void deleteWebFrameworkJSONParser(JSONParser parser);

/**
 * @brief Get string from WebFrameworkString
 * @param string WebFrameworkString
 * @return String data
 */
const char* getDataFromString(WebFrameworkString string);

size_t findLastChar(char* ptr, char c);
