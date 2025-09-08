#pragma once

#include "JSONObject.h"

/**
 * @brief Create JSONParser
 * @param builder Parser. Delete with deleteWebFrameworkJSONParser function
 * @return Error if occurred
 */
WebFrameworkException createJSONParser(JSONParser* parser);

/**
 * @brief Copy Parser
 * @param builder Result. Delete with deleteWebFrameworkJSONBuilder function
 * @param other Other Parser
 * @return Error if occurred
 */
WebFrameworkException copyJSONParser(JSONParser* parser, const JSONParser* other);

/**
 * @brief Create JSONParser from JSON formatted string
 * @param builder Parser. Delete with deleteWebFrameworkJSONParser function
 * @return Error if occurred
 */
WebFrameworkException createJSONParserFromString(const char* jsonString, JSONParser* parser);

/**
 * @brief Get JSONObject
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JSONObject
 * @return Error if occurred
 */
WebFrameworkException getJSONParserObject(JSONParser implementation, const char* key, bool recursive, JSONObject_t* result);

/**
 * @brief Get string
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result string
 * @return Error if occurred
 */
WebFrameworkException getJSONParserString(JSONParser implementation, const char* key, bool recursive, const char** result);

/**
 * @brief Get integer
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result integer
 * @return Error if occurred
 */
WebFrameworkException getJSONParserInteger(JSONParser implementation, const char* key, bool recursive, int64_t* result);

/**
 * @brief Get unsigned integer
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result unsigned integer
 * @return Error if occurred
 */
WebFrameworkException getJSONParserUnsignedInteger(JSONParser implementation, const char* key, bool recursive, uint64_t* result);

/**
 * @brief Get double
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result double
 * @return Error if occurred
 */
WebFrameworkException getJSONParserDouble(JSONParser implementation, const char* key, bool recursive, double* result);

/**
 * @brief Get bool
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result bool
 * @return Error if occurred
 */
WebFrameworkException getJSONParserBoolean(JSONParser implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get NULL
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Is NULL
 * @return Error if occurred
 */
WebFrameworkException getJSONParserNull(JSONParser implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get JSONArray
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JSONArray
 * @return Error if occurred
 */
WebFrameworkException getJSONParserArray(JSONParser implementation, const char* key, JSONArray_t* array, bool recursive);

/**
 * @brief Try to get JSONObject
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param value Result JSONObject
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserObject(JSONParser implementation, const char* key, JSONObject_t* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get string
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param value Result string
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserString(JSONParser implementation, const char* key, const char** value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get integer
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param value Result integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserInteger(JSONParser implementation, const char* key, int64_t* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get unsigned integer
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param value Result unsigned integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserUnsignedInteger(JSONParser implementation, const char* key, uint64_t* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get double
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param value Result double
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserDouble(JSONParser implementation, const char* key, double* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get bool
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param value Result bool
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserBoolean(JSONParser implementation, const char* key, bool* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get NULL
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserNull(JSONParser implementation, const char* key, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get JSONArray
 * @param implementation JSONParser instance
 * @param key JSON key
 * @param value Result JSONArray
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has JSON field
 */
bool tryGetJSONParserArray(JSONParser implementation, const char* key, JSONArray_t* array, bool recursive, WebFrameworkException* exception);
