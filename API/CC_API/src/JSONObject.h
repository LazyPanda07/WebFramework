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

void __appendJSONArrayObject(JSONArray* array, void* value);

JSONArray createJSONArray(size_t capacity);

WebFrameworkException appendJSONArrayObject(JSONArray* array, JSONObject* value);

WebFrameworkException appendJSONArrayString(JSONArray* array, const char* value);

WebFrameworkException appendJSONArrayInteger(JSONArray* array, int64_t value);

WebFrameworkException appendJSONArrayUnsignedInteger(JSONArray* array, uint64_t value);

WebFrameworkException appendJSONArrayDouble(JSONArray* array, double value);

WebFrameworkException appendJSONArrayBoolean(JSONArray* array, bool value);

WebFrameworkException appendJSONArrayNull(JSONArray* array);

WebFrameworkException appendJSONArrayArray(JSONArray* array, const JSONArray* objects);

JSONArray deleteJSONArray(JSONArray* array);

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

inline size_t __getIndex(JSONArray* array)
{
	if (array->size == array->capacity)
	{
		JSONObject* temp = array->data;

		array->capacity *= 2;
		array->data = (JSONObject*)malloc(array->capacity * sizeof(JSONObject));

		memcpy(array->data, temp, array->size * sizeof(JSONObject));

		free(temp);
	}

	return array->size++;
}

inline void __appendJSONArrayObject(JSONArray* array, void* value)
{
	size_t index = __getIndex(array);

	array->data[index].implementation = value;
	array->data[index].weak = true;
}

inline JSONArray createJSONArray(size_t capacity)
{
	JSONArray result =
	{
		.data = (JSONObject*)malloc(capacity * sizeof(JSONObject)),
		.size = 0,
		.capacity = capacity
	};

	memset(result.data, 0, capacity * sizeof(JSONObject));

	return result;
}

inline WebFrameworkException appendJSONArrayObject(JSONArray* array, JSONObject* value)
{
	JSONObject object;
	WebFrameworkException exception = createJSONObject(&object);

	if (exception)
	{
		return exception;
	}

	exception = setJSONObjectObject(&object, "", value);

	if (exception)
	{
		return exception;
	}

	array->data[__getIndex(array)] = object;

	return exception;
}

inline WebFrameworkException appendJSONArrayString(JSONArray* array, const char* value)
{
	JSONObject object;
	WebFrameworkException exception = createJSONObject(&object);

	if (exception)
	{
		return exception;
	}

	exception = setJSONObjectString(&object, "", value);

	if (exception)
	{
		return exception;
	}

	array->data[__getIndex(array)] = object;

	return exception;
}

inline WebFrameworkException appendJSONArrayInteger(JSONArray* array, int64_t value)
{
	JSONObject object;
	WebFrameworkException exception = createJSONObject(&object);

	if (exception)
	{
		return exception;
	}

	exception = setJSONObjectInteger(&object, "", value);

	if (exception)
	{
		return exception;
	}

	array->data[__getIndex(array)] = object;

	return exception;
}

inline WebFrameworkException appendJSONArrayUnsignedInteger(JSONArray* array, uint64_t value)
{
	JSONObject object;
	WebFrameworkException exception = createJSONObject(&object);

	if (exception)
	{
		return exception;
	}

	exception = setJSONObjectUnsignedInteger(&object, "", value);

	if (exception)
	{
		return exception;
	}

	array->data[__getIndex(array)] = object;

	return exception;
}

inline WebFrameworkException appendJSONArrayDouble(JSONArray* array, double value)
{
	JSONObject object;
	WebFrameworkException exception = createJSONObject(&object);

	if (exception)
	{
		return exception;
	}

	exception = setJSONObjectDouble(&object, "", value);

	if (exception)
	{
		return exception;
	}

	array->data[__getIndex(array)] = object;

	return exception;
}

inline WebFrameworkException appendJSONArrayBoolean(JSONArray* array, bool value)
{
	JSONObject object;
	WebFrameworkException exception = createJSONObject(&object);

	if (exception)
	{
		return exception;
	}

	exception = setJSONObjectBoolean(&object, "", value);

	if (exception)
	{
		return exception;
	}

	array->data[__getIndex(array)] = object;

	return exception;
}

inline WebFrameworkException appendJSONArrayNull(JSONArray* array)
{
	JSONObject object;
	WebFrameworkException exception = createJSONObject(&object);

	if (exception)
	{
		return exception;
	}

	exception = setJSONObjectNull(&object, "");

	if (exception)
	{
		return exception;
	}

	array->data[__getIndex(array)] = object;

	return exception;
}

inline WebFrameworkException appendJSONArrayArray(JSONArray* array, const JSONArray* objects)
{
	WebFrameworkException exception;

	for (size_t i = 0; i < objects->size; i++)
	{
		exception = appendJSONArrayObject(array, &objects->data[i]);

		if (exception)
		{
			break;
		}
	}

	return exception;
}

inline JSONArray deleteJSONArray(JSONArray* array)
{
	for (size_t i = 0; i < array->size; i++)
	{
		deleteJSONObject(&array->data[i]);
	}

	free(array->data);
	array->capacity = 0;
	array->size = 0;
}

inline WebFrameworkException createJSONObject(JSONObject* jsonObject)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONObject)(void* _, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJSONObject, NULL, &exception);
	jsonObject->weak = false;

	return exception;
}

inline WebFrameworkException copyJSONObject(JSONObject* jsonObject, const JSONObject* other)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONObject)(void* other, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJSONObject, other->implementation, &exception);
	jsonObject->weak = false;

	return exception;
}

inline WebFrameworkException setJSONObjectObject(JSONObject* jsonObject, const char* key, JSONObject* object)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectObject)(void* implementation, const char* key, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectObject, key, object->implementation, &exception);

	return exception;
}

inline WebFrameworkException setJSONObjectString(JSONObject* jsonObject, const char* key, const char* value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectString)(void* implementation, const char* key, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectString, key, value, &exception);

	return exception;
}

inline WebFrameworkException setJSONObjectInteger(JSONObject* jsonObject, const char* key, int64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectInteger)(void* implementation, const char* key, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectInteger, key, value, &exception);

	return exception;
}

inline WebFrameworkException setJSONObjectUnsignedInteger(JSONObject* jsonObject, const char* key, uint64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectUnsignedInteger)(void* implementation, const char* key, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectUnsignedInteger, key, value, &exception);

	return exception;
}

inline WebFrameworkException setJSONObjectDouble(JSONObject* jsonObject, const char* key, double value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectDouble)(void* implementation, const char* key, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectDouble, key, value, &exception);

	return exception;
}

inline WebFrameworkException setJSONObjectBoolean(JSONObject* jsonObject, const char* key, bool value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectBoolean)(void* implementation, const char* key, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectBoolean, key, value, &exception);

	return exception;
}

inline WebFrameworkException setJSONObjectNull(JSONObject* jsonObject, const char* key)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectNull)(void* implementation, const char* key, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectNull, key, &exception);

	return exception;
}

inline WebFrameworkException setJSONObjectArray(JSONObject* jsonObject, const char* key, const JSONArray* array)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;
	void* buffer = malloc(array->size * sizeof(void*));
	void** value = &buffer;

	typedef void (*setJSONObjectArray)(void* implementation, const char* key, const void** value, void** exception);

	for (size_t i = 0; i < array->size; i++)
	{
		value[i] = array->data[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectArray, key, value, &exception);

	free(buffer);

	return exception;
}

inline void deleteJSONObject(JSONObject* jsonObject)
{
	typedef void* (*deleteWebFrameworkJSONObject)(void* implementation);

	if (!jsonObject->weak && jsonObject->implementation)
	{
		CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJSONObject, jsonObject->implementation);

		jsonObject->implementation = NULL;
	}
}
