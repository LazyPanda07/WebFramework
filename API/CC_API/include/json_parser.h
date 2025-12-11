#pragma once

#include "JsonObject.h"

/**
 * @brief Create json_parser_t
 * @param builder Parser. Delete with deleteWebFrameworkJsonParser function
 * @return Error if occurred
 */
web_framework_exception_t createJsonParser(json_parser_t* parser);

/**
 * @brief Copy Parser
 * @param builder Result. Delete with deleteWebFrameworkJsonBuilder function
 * @param other Other Parser
 * @return Error if occurred
 */
web_framework_exception_t copyJsonParser(json_parser_t* parser, const json_parser_t* other);

/**
 * @brief Create json_parser_t from Json formatted string
 * @param builder Parser. Delete with deleteWebFrameworkJsonParser function
 * @return Error if occurred
 */
web_framework_exception_t createJsonParserFromString(const char* jsonString, json_parser_t* parser);

/**
 * @brief Get JsonObject
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JsonObject
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserObject(json_parser_t implementation, const char* key, bool recursive, JsonObject_t* result);

/**
 * @brief Get string
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result string
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserString(json_parser_t implementation, const char* key, bool recursive, const char** result);

/**
 * @brief Get integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result integer
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserInteger(json_parser_t implementation, const char* key, bool recursive, int64_t* result);

/**
 * @brief Get unsigned integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result unsigned integer
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserUnsignedInteger(json_parser_t implementation, const char* key, bool recursive, uint64_t* result);

/**
 * @brief Get double
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result double
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserDouble(json_parser_t implementation, const char* key, bool recursive, double* result);

/**
 * @brief Get bool
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result bool
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserBoolean(json_parser_t implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get NULL
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Is NULL
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserNull(json_parser_t implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get JsonArray
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JsonArray
 * @return Error if occurred
 */
web_framework_exception_t getJsonParserArray(json_parser_t implementation, const char* key, JsonObject_t* array, size_t* arraySize, bool recursive);

/**
 * @brief Try to get JsonObject
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result JsonObject
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserObject(json_parser_t implementation, const char* key, JsonObject_t* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get string
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result string
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserString(json_parser_t implementation, const char* key, const char** value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserInteger(json_parser_t implementation, const char* key, int64_t* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get unsigned integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result unsigned integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserUnsignedInteger(json_parser_t implementation, const char* key, uint64_t* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get double
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result double
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserDouble(json_parser_t implementation, const char* key, double* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get bool
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result bool
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserBoolean(json_parser_t implementation, const char* key, bool* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get NULL
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserNull(json_parser_t implementation, const char* key, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get JsonArray
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result JsonArray
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserArray(json_parser_t implementation, const char* key, JsonObject_t* array, size_t* arraySize, bool recursive, web_framework_exception_t* exception);
