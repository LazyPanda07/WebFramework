#pragma once

#include "JsonObject.h"

/**
 * @brief Create JSONBuilder
 * @param builder JSONBuilder. Delete with deleteWebFrameworkJSONBuilder function
 * @return Error if occurred
 */
WebFrameworkException createJSONBuilder(JSONBuilder* builder);

/**
 * @brief Copy JSONBuilder
 * @param builder Result. Delete with deleteWebFrameworkJSONBuilder function
 * @param other Other JSONBuilder
 * @return Error if occurred
 */
WebFrameworkException copyJSONBuilder(JSONBuilder* builder, JSONBuilder other);

/**
 * @brief Get JSON formatted text
 * @param implementation JSONBuilder instance
 * @param result JSON formatted text. Delete with deleteWebFrameworkString function
 * @return 
 */
WebFrameworkException buildJSONBuilder(JSONBuilder implementation, WebFrameworkString* result);

/**
 * @brief Add JSONObject
 * @param implementation JSONBuilder instance 
 * @param key JSON key
 * @param object JSON object
 * @return Error if occurred
 */
WebFrameworkException appendJSONBuilderObject(JSONBuilder implementation, const char* key, JSONObject_t* object);

/**
 * @brief Add string
 * @param implementation JSONBuilder instance
 * @param key JSON key
 * @param value String value
 * @return Error if occurred
 */
WebFrameworkException appendJSONBuilderString(JSONBuilder implementation, const char* key, const char* value);

/**
 * @brief Add integer
 * @param implementation JSONBuilder instance
 * @param key JSON key
 * @param value Integer value
 * @return 
 */
WebFrameworkException appendJSONBuilderInteger(JSONBuilder implementation, const char* key, int64_t value);

/**
 * @brief Add unsigned integer
 * @param implementation JSONBuilder instance
 * @param key JSON key
 * @param value Unsigned integer value
 * @return Error if occurred
 */
WebFrameworkException appendJSONBuilderUnsignedInteger(JSONBuilder implementation, const char* key, uint64_t value);

/**
 * @brief Add double
 * @param implementation JSONBuilder instance
 * @param key JSON key
 * @param value Double value
 * @return Error if occurred
 */
WebFrameworkException appendJSONBuilderDouble(JSONBuilder implementation, const char* key, double value);

/**
 * @brief Add bool
 * @param implementation JSONBuilder instance
 * @param key JSON key
 * @param value Bool value
 * @return Error if occurred
 */
WebFrameworkException appendJSONBuilderBoolean(JSONBuilder implementation, const char* key, bool value);

/**
 * @brief Add NULL
 * @param implementation JSONBuilder instance
 * @param key JSON key
 * @return Error if occurred
 */
WebFrameworkException appendJSONBuilderNull(JSONBuilder implementation, const char* key);

/**
 * @brief Add JSON array
 * @param implementation JSONBuilder instance
 * @param key JSON key
 * @param value JSON array value
 * @return Error if occurred
 */
WebFrameworkException appendJSONBuilderArray(JSONBuilder implementation, const char* key, const JSONObject_t* array, size_t arraySize);
