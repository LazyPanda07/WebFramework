#pragma once

#include "JsonObject.h"

/**
 * @brief Create JsonParser
 * @param builder Parser. Delete with deleteWebFrameworkJsonParser function
 * @return Error if occurred
 */
WebFrameworkException createJsonParser(JsonParser* parser);

/**
 * @brief Copy Parser
 * @param builder Result. Delete with deleteWebFrameworkJsonBuilder function
 * @param other Other Parser
 * @return Error if occurred
 */
WebFrameworkException copyJsonParser(JsonParser* parser, const JsonParser* other);

/**
 * @brief Create JsonParser from Json formatted string
 * @param builder Parser. Delete with deleteWebFrameworkJsonParser function
 * @return Error if occurred
 */
WebFrameworkException createJsonParserFromString(const char* jsonString, JsonParser* parser);

/**
 * @brief Get JsonObject
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JsonObject
 * @return Error if occurred
 */
WebFrameworkException getJsonParserObject(JsonParser implementation, const char* key, bool recursive, JsonObject_t* result);

/**
 * @brief Get string
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result string
 * @return Error if occurred
 */
WebFrameworkException getJsonParserString(JsonParser implementation, const char* key, bool recursive, const char** result);

/**
 * @brief Get integer
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result integer
 * @return Error if occurred
 */
WebFrameworkException getJsonParserInteger(JsonParser implementation, const char* key, bool recursive, int64_t* result);

/**
 * @brief Get unsigned integer
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result unsigned integer
 * @return Error if occurred
 */
WebFrameworkException getJsonParserUnsignedInteger(JsonParser implementation, const char* key, bool recursive, uint64_t* result);

/**
 * @brief Get double
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result double
 * @return Error if occurred
 */
WebFrameworkException getJsonParserDouble(JsonParser implementation, const char* key, bool recursive, double* result);

/**
 * @brief Get bool
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result bool
 * @return Error if occurred
 */
WebFrameworkException getJsonParserBoolean(JsonParser implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get NULL
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Is NULL
 * @return Error if occurred
 */
WebFrameworkException getJsonParserNull(JsonParser implementation, const char* key, bool recursive, bool* result);

/**
 * @brief Get JsonArray
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param result Result JsonArray
 * @return Error if occurred
 */
WebFrameworkException getJsonParserArray(JsonParser implementation, const char* key, JsonObject_t* array, size_t* arraySize, bool recursive);

/**
 * @brief Try to get JsonObject
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param value Result JsonObject
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserObject(JsonParser implementation, const char* key, JsonObject_t* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get string
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param value Result string
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserString(JsonParser implementation, const char* key, const char** value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get integer
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param value Result integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserInteger(JsonParser implementation, const char* key, int64_t* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get unsigned integer
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param value Result unsigned integer
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserUnsignedInteger(JsonParser implementation, const char* key, uint64_t* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get double
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param value Result double
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserDouble(JsonParser implementation, const char* key, double* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get bool
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param value Result bool
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserBoolean(JsonParser implementation, const char* key, bool* value, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get NULL
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserNull(JsonParser implementation, const char* key, bool recursive, WebFrameworkException* exception);

/**
 * @brief Try to get JsonArray
 * @param implementation JsonParser instance
 * @param key JSON key
 * @param value Result JsonArray
 * @param recursive Search recursively
 * @param exception Error if occurred
 * @return Has Json field
 */
bool tryGetJsonParserArray(JsonParser implementation, const char* key, JsonObject_t* array, size_t* arraySize, bool recursive, WebFrameworkException* exception);
