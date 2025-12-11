#pragma once

#include "JsonObject.h"

/**
 * @brief Create JsonBuilder
 * @param builder JsonBuilder. Delete with deleteWebFrameworkJsonBuilder function
 * @return Error if occurred
 */
WebFrameworkException createJsonBuilder(JsonBuilder* builder);

/**
 * @brief Copy JsonBuilder
 * @param builder Result. Delete with deleteWebFrameworkJsonBuilder function
 * @param other Other JsonBuilder
 * @return Error if occurred
 */
WebFrameworkException copyJsonBuilder(JsonBuilder* builder, JsonBuilder other);

/**
 * @brief Get Json formatted text
 * @param implementation JsonBuilder instance
 * @param result Json formatted text. Delete with deleteWebFrameworkString function
 * @return 
 */
WebFrameworkException buildJsonBuilder(JsonBuilder implementation, WebFrameworkString* result);

/**
 * @brief Add JsonObject
 * @param implementation JsonBuilder instance 
 * @param key JSON key
 * @param object Json object
 * @return Error if occurred
 */
WebFrameworkException appendJsonBuilderObject(JsonBuilder implementation, const char* key, JsonObject_t* object);

/**
 * @brief Add string
 * @param implementation JsonBuilder instance
 * @param key JSON key
 * @param value String value
 * @return Error if occurred
 */
WebFrameworkException appendJsonBuilderString(JsonBuilder implementation, const char* key, const char* value);

/**
 * @brief Add integer
 * @param implementation JsonBuilder instance
 * @param key JSON key
 * @param value Integer value
 * @return 
 */
WebFrameworkException appendJsonBuilderInteger(JsonBuilder implementation, const char* key, int64_t value);

/**
 * @brief Add unsigned integer
 * @param implementation JsonBuilder instance
 * @param key JSON key
 * @param value Unsigned integer value
 * @return Error if occurred
 */
WebFrameworkException appendJsonBuilderUnsignedInteger(JsonBuilder implementation, const char* key, uint64_t value);

/**
 * @brief Add double
 * @param implementation JsonBuilder instance
 * @param key JSON key
 * @param value Double value
 * @return Error if occurred
 */
WebFrameworkException appendJsonBuilderDouble(JsonBuilder implementation, const char* key, double value);

/**
 * @brief Add bool
 * @param implementation JsonBuilder instance
 * @param key JSON key
 * @param value Bool value
 * @return Error if occurred
 */
WebFrameworkException appendJsonBuilderBoolean(JsonBuilder implementation, const char* key, bool value);

/**
 * @brief Add NULL
 * @param implementation JsonBuilder instance
 * @param key JSON key
 * @return Error if occurred
 */
WebFrameworkException appendJsonBuilderNull(JsonBuilder implementation, const char* key);

/**
 * @brief Add Json array
 * @param implementation JsonBuilder instance
 * @param key JSON key
 * @param value Json array value
 * @return Error if occurred
 */
WebFrameworkException appendJsonBuilderArray(JsonBuilder implementation, const char* key, const JsonObject_t* array, size_t arraySize);
