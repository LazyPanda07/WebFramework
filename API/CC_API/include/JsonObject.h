#pragma once

#include "DLLHandler.h"

/**
 * @brief JSONObject
 */
typedef struct JSONObject
{
	void* implementation;
	bool weak;
} JSONObject_t;

/**
 * @brief Create JSONObject. Delete with deleteWebFrameworkJSONObject function
 * @param jsonObject JSONObject
 * @return Error if occurred
 */
WebFrameworkException createJSONObject(JSONObject_t* jsonObject);

/**
 * @brief Copy JSONObject.
 * @param jsonObject Result. Delete with deleteWebFrameworkJSONObject function
 * @param other Other JSONObject
 * @return Error if occurred
 */
WebFrameworkException copyJSONObject(JSONObject_t* jsonObject, const JSONObject_t* other);

/**
 * @brief Set JSONObject to JSONObject
 * @param jsonObject JSONObject instance
 * @param object JSONObject value
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectObject(JSONObject_t* jsonObject, JSONObject_t* object);

/**
 * @brief Set string to JSONObject
 * @param jsonObject JSONObject instance
 * @param value String value
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectString(JSONObject_t* jsonObject, const char* value);

/**
 * @brief Set integer to JSONObject
 * @param jsonObject JSONObject instance
 * @param value Integer value
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectInteger(JSONObject_t* jsonObject, int64_t value);

/**
 * @brief Set unsigned integer to JSONObject
 * @param jsonObject JSONObject instance
 * @param value Unsigned integer value
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectUnsignedInteger(JSONObject_t* jsonObject, uint64_t value);

/**
 * @brief Set double to JSONObject
 * @param jsonObject JSONObject instance
 * @param value Double value
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectDouble(JSONObject_t* jsonObject, double value);

/**
 * @brief Set bool to JSONObject
 * @param jsonObject JSONObject instance
 * @param value Bool value
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectBoolean(JSONObject_t* jsonObject, bool value);

/**
 * @brief Set NULL to JSONObject
 * @param jsonObject JSONObject instance
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectNull(JSONObject_t* jsonObject);

/**
 * @brief Set JSONArray to JSONObject
 * @param jsonObject JSONObject instance
 * @param value JSONArray value
 * @return Error if occurred
 */
WebFrameworkException setJSONObjectArray(JSONObject_t* jsonObject, const JSONObject_t* array, size_t arraySize);

/**
 * @brief Add key to JSON object and get associated object
 * @param jsonObject JSONObject instance
 * @param key JSON key
 * @param result Associated JSON object
 * @return 
 */
WebFrameworkException assignJSONObject(JSONObject_t* jsonObject, const char* key, JSONObject_t* result);

/**
 * @brief Add JSON object to an array and get associated object
 * @param jsonObject JSONObject instance
 * @param result Associated JSON object
 * @return
 */
WebFrameworkException emplaceJSONObject(JSONObject_t* jsonObject, JSONObject_t* result);

/**
 * @brief Obtains the size of a JSON object and writes it to the provided output parameter.
 * @param jsonObject Pointer to the JSON object whose size will be determined.
 * @param size Pointer to a size_t where the computed size will be stored on success. If jsonObject is map or array it returns actual size, ULLONG_MAX otherwise
 * @return A WebFrameworkException value indicating success or failure. On success, *size is set to the object's size.
 */
WebFrameworkException sizeJSONObject(JSONObject_t* jsonObject, size_t* size);

/**
 * @brief Delete JSONObject
 * @param jsonObject JSONObject instance
 */
void deleteJSONObject(JSONObject_t* jsonObject);
