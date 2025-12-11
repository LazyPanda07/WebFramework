#pragma once

#include "DLLHandler.h"

/**
 * @brief JsonObject
 */
typedef struct JsonObject
{
	void* implementation;
	bool weak;
} JsonObject_t;

/**
 * @brief Create JsonObject. Delete with deleteWebFrameworkJsonObject function
 * @param jsonObject JsonObject
 * @return Error if occurred
 */
web_framework_exception_t createJsonObject(JsonObject_t* jsonObject);

/**
 * @brief Copy JsonObject.
 * @param jsonObject Result. Delete with deleteWebFrameworkJsonObject function
 * @param other Other JsonObject
 * @return Error if occurred
 */
web_framework_exception_t copyJsonObject(JsonObject_t* jsonObject, const JsonObject_t* other);

/**
 * @brief Set JsonObject to JsonObject
 * @param jsonObject JsonObject instance
 * @param object JsonObject value
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectObject(JsonObject_t* jsonObject, JsonObject_t* object);

/**
 * @brief Set string to JsonObject
 * @param jsonObject JsonObject instance
 * @param value String value
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectString(JsonObject_t* jsonObject, const char* value);

/**
 * @brief Set integer to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Integer value
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectInteger(JsonObject_t* jsonObject, int64_t value);

/**
 * @brief Set unsigned integer to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Unsigned integer value
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectUnsignedInteger(JsonObject_t* jsonObject, uint64_t value);

/**
 * @brief Set double to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Double value
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectDouble(JsonObject_t* jsonObject, double value);

/**
 * @brief Set bool to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Bool value
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectBoolean(JsonObject_t* jsonObject, bool value);

/**
 * @brief Set NULL to JsonObject
 * @param jsonObject JsonObject instance
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectNull(JsonObject_t* jsonObject);

/**
 * @brief Set JsonArray to JsonObject
 * @param jsonObject JsonObject instance
 * @param value JsonArray value
 * @return Error if occurred
 */
web_framework_exception_t setJsonObjectArray(JsonObject_t* jsonObject, const JsonObject_t* array, size_t arraySize);

/**
 * @brief Add key to Json object and get associated object
 * @param jsonObject JsonObject instance
 * @param key JSON key
 * @param result Associated Json object
 * @return 
 */
web_framework_exception_t assignJsonObject(JsonObject_t* jsonObject, const char* key, JsonObject_t* result);

/**
 * @brief Add Json object to an array and get associated object
 * @param jsonObject JsonObject instance
 * @param result Associated Json object
 * @return
 */
web_framework_exception_t emplaceJsonObject(JsonObject_t* jsonObject, JsonObject_t* result);

/**
 * @brief Obtains the size of a Json object and writes it to the provided output parameter.
 * @param jsonObject Pointer to the Json object whose size will be determined.
 * @param size Pointer to a size_t where the computed size will be stored on success. If jsonObject is map or array it returns actual size, ULLONG_MAX otherwise
 * @return A web_framework_exception_t value indicating success or failure. On success, *size is set to the object's size.
 */
web_framework_exception_t sizeJsonObject(JsonObject_t* jsonObject, size_t* size);

/**
 * @brief Delete JsonObject
 * @param jsonObject JsonObject instance
 */
void deleteJsonObject(JsonObject_t* jsonObject);
