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
 * @brief Array of JSONObject
 */
typedef struct JSONArray_t
{
	JSONObject_t* data;
	size_t size;
	size_t capacity;
} JSONArray_t;

size_t __getIndex(JSONArray_t* array);

/**
 * @brief Create Array of JSONObject. Delete with deleteJSONArray function
 * @param capacity Start capacity
 * @return JSONArray
 */
JSONArray_t createJSONArray(size_t capacity);

/**
 * @brief Add JSONObject to JSONArray
 * @param array JSONArray
 * @param value JSONObject
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayObject(JSONArray_t* array, JSONObject_t* value);

/**
 * @brief Add string to JSONArray
 * @param array JSONArray
 * @param value String
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayString(JSONArray_t* array, const char* value);

/**
 * @brief Add integer to JSONArray
 * @param array JSONArray
 * @param value Integer
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayInteger(JSONArray_t* array, int64_t value);

/**
 * @brief Add string to JSONArray
 * @param array JSONArray
 * @param value String
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayUnsignedInteger(JSONArray_t* array, uint64_t value);

/**
 * @brief Add double to JSONArray
 * @param array JSONArray
 * @param value Double
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayDouble(JSONArray_t* array, double value);

/**
 * @brief Add bool to JSONArray
 * @param array JSONArray
 * @param value Bool
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayBoolean(JSONArray_t* array, bool value);

/**
 * @brief Add NULL to JSONArray
 * @param array JSONArray
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayNull(JSONArray_t* array);

/**
 * @brief Add JSONArray to JSONArray
 * @param array JSONArray
 * @param value JSONArray
 * @return Error if occurred
 */
WebFrameworkException appendJSONArrayArray(JSONArray_t* array, const JSONArray_t* objects);

/**
 * @brief Delete JSONArray
 * @param array JSONArray instance
 */
void deleteJSONArray(JSONArray_t* array);

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
WebFrameworkException setJSONObjectArray(JSONObject_t* jsonObject, const JSONArray_t* array);

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
 * @brief Delete JSONObject
 * @param jsonObject JSONObject instance
 */
void deleteJSONObject(JSONObject_t* jsonObject);
