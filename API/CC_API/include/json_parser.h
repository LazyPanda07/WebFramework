#pragma once

#include "json_object.h"

/**
 * @brief Create json_parser_t
 * @param builder Parser. Delete with deleteWebFrameworkJsonParser function
 * @return Error if occurred
 */
web_framework_exception_t wf_create_json_parser(json_parser_t* parser);

/**
 * @brief Copy Parser
 * @param builder Result. Delete with deleteWebFrameworkJsonBuilder function
 * @param other Other Parser
 * @return Error if occurred
 */
web_framework_exception_t wf_copy_json_parser(json_parser_t* parser, const json_parser_t* other);

/**
 * @brief Create json_parser_t from Json formatted string
 * @param builder Parser. Delete with deleteWebFrameworkJsonParser function
 * @return Error if occurred
 */
web_framework_exception_t wf_create_json_parser_from_string(const char* jsonString, json_parser_t* parser);

/**
 * @brief Get JsonObject
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JsonObject
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_object(json_parser_t implementation, const char* key, bool recursive, json_object_t* result);

/**
 * @brief Get string
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result string
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_string(json_parser_t implementation, const char* key, bool recursive, const char** result);

/**
 * @brief Get integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result integer
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_integer(json_parser_t implementation, const char* key, bool recursive, int64_t* result);

/**
 * @brief Get unsigned integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result unsigned integer
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_unsigned_integer(json_parser_t implementation, const char* key, bool recursive, uint64_t* result);

/**
 * @brief Get double
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result double
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_double(json_parser_t implementation, const char* key, bool recursive, double* result);

/**
 * @brief Get bool
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result bool
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_boolean(json_parser_t implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get NULL
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Is NULL
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_null(json_parser_t implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get JsonArray
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JsonArray
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_parser_array(json_parser_t implementation, const char* key, json_object_t* array, size_t* arraySize, bool recursive);

/**
 * @brief Try to get JsonObject
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result JsonObject
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_object(json_parser_t implementation, const char* key, json_object_t* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get string
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result string
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_string(json_parser_t implementation, const char* key, const char** value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_integer(json_parser_t implementation, const char* key, int64_t* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get unsigned integer
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result unsigned integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_unsigned_integer(json_parser_t implementation, const char* key, uint64_t* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get double
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result double
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_double(json_parser_t implementation, const char* key, double* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get bool
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result bool
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_boolean(json_parser_t implementation, const char* key, bool* value, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get NULL
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_null(json_parser_t implementation, const char* key, bool recursive, web_framework_exception_t* exception);

/**
 * @brief Try to get JsonArray
 * @param implementation json_parser_t instance
 * @param key JSON key
 * @param value Result JsonArray
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool wf_try_get_json_parser_array(json_parser_t implementation, const char* key, json_object_t* array, size_t* arraySize, bool recursive, web_framework_exception_t* exception);
