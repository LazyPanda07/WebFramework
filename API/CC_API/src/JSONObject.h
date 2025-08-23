#pragma once

#include "DLLHandler.h"

typedef struct
{
	void* implementation;
	bool weak;
} JSONObject;

typedef struct
{
	JSONObject* data;
	size_t size;
	size_t capacity;
} JSONArray;

size_t __getIndex(JSONArray* array);

JSONArray createJSONArray(size_t capacity);

WebFrameworkException appendJSONArrayObject(JSONArray* array, JSONObject* value);

WebFrameworkException appendJSONArrayString(JSONArray* array, const char* value);

WebFrameworkException appendJSONArrayInteger(JSONArray* array, int64_t value);

WebFrameworkException appendJSONArrayUnsignedInteger(JSONArray* array, uint64_t value);

WebFrameworkException appendJSONArrayDouble(JSONArray* array, double value);

WebFrameworkException appendJSONArrayBoolean(JSONArray* array, bool value);

WebFrameworkException appendJSONArrayNull(JSONArray* array);

WebFrameworkException appendJSONArrayArray(JSONArray* array, const JSONArray* objects);

void deleteJSONArray(JSONArray* array);

WebFrameworkException createJSONObject(JSONObject* jsonObject);

WebFrameworkException copyJSONObject(JSONObject* jsonObject, const JSONObject* other);

WebFrameworkException setJSONObjectObject(JSONObject* jsonObject, const char* key, JSONObject* object);

WebFrameworkException setJSONObjectString(JSONObject* jsonObject, const char* key, const char* value);

WebFrameworkException setJSONObjectInteger(JSONObject* jsonObject, const char* key, int64_t value);

WebFrameworkException setJSONObjectUnsignedInteger(JSONObject* jsonObject, const char* key, uint64_t value);

WebFrameworkException setJSONObjectDouble(JSONObject* jsonObject, const char* key, double value);

WebFrameworkException setJSONObjectBoolean(JSONObject* jsonObject, const char* key, bool value);

WebFrameworkException setJSONObjectNull(JSONObject* jsonObject, const char* key);

WebFrameworkException setJSONObjectArray(JSONObject* jsonObject, const char* key, const JSONArray* array);

void deleteJSONObject(JSONObject* jsonObject);
