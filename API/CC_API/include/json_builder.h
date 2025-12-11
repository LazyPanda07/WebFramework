#pragma once

#include "JsonObject.h"

/**
 * @brief Create json_builder_t
 * @param builder json_builder_t. Delete with deleteWebFrameworkJsonBuilder function
 * @return Error if occurred
 */
web_framework_exception_t createJsonBuilder(json_builder_t* builder);

/**
 * @brief Copy json_builder_t
 * @param builder Result. Delete with deleteWebFrameworkJsonBuilder function
 * @param other Other json_builder_t
 * @return Error if occurred
 */
web_framework_exception_t copyJsonBuilder(json_builder_t* builder, json_builder_t other);

/**
 * @brief Get Json formatted text
 * @param implementation json_builder_t instance
 * @param result Json formatted text. Delete with deleteWebFrameworkString function
 * @return 
 */
web_framework_exception_t buildJsonBuilder(json_builder_t implementation, web_framework_string_t* result);

/**
 * @brief Add JsonObject
 * @param implementation json_builder_t instance 
 * @param key JSON key
 * @param object Json object
 * @return Error if occurred
 */
web_framework_exception_t appendJsonBuilderObject(json_builder_t implementation, const char* key, JsonObject_t* object);

/**
 * @brief Add string
 * @param implementation json_builder_t instance
 * @param key JSON key
 * @param value String value
 * @return Error if occurred
 */
web_framework_exception_t appendJsonBuilderString(json_builder_t implementation, const char* key, const char* value);

/**
 * @brief Add integer
 * @param implementation json_builder_t instance
 * @param key JSON key
 * @param value Integer value
 * @return 
 */
web_framework_exception_t appendJsonBuilderInteger(json_builder_t implementation, const char* key, int64_t value);

/**
 * @brief Add unsigned integer
 * @param implementation json_builder_t instance
 * @param key JSON key
 * @param value Unsigned integer value
 * @return Error if occurred
 */
web_framework_exception_t appendJsonBuilderUnsignedInteger(json_builder_t implementation, const char* key, uint64_t value);

/**
 * @brief Add double
 * @param implementation json_builder_t instance
 * @param key JSON key
 * @param value Double value
 * @return Error if occurred
 */
web_framework_exception_t appendJsonBuilderDouble(json_builder_t implementation, const char* key, double value);

/**
 * @brief Add bool
 * @param implementation json_builder_t instance
 * @param key JSON key
 * @param value Bool value
 * @return Error if occurred
 */
web_framework_exception_t appendJsonBuilderBoolean(json_builder_t implementation, const char* key, bool value);

/**
 * @brief Add NULL
 * @param implementation json_builder_t instance
 * @param key JSON key
 * @return Error if occurred
 */
web_framework_exception_t appendJsonBuilderNull(json_builder_t implementation, const char* key);

/**
 * @brief Add Json array
 * @param implementation json_builder_t instance
 * @param key JSON key
 * @param value Json array value
 * @return Error if occurred
 */
web_framework_exception_t appendJsonBuilderArray(json_builder_t implementation, const char* key, const JsonObject_t* array, size_t arraySize);
