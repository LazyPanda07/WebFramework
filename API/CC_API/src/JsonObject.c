#include "JsonObject.h"

size_t __getIndex(JSONArray_t* array)
{
	if (array->size == array->capacity)
	{
		JSONObject_t* temp = array->data;

		array->capacity *= 2;
		array->data = (JSONObject_t*)malloc(array->capacity * sizeof(JSONObject_t));

		if (array->data)
		{
			memcpy(array->data, temp, array->size * sizeof(JSONObject_t));
		}

		free(temp);
	}

	return array->size++;
}

JSONArray_t createJSONArray(size_t capacity)
{
	JSONArray_t result =
	{
		.data = (JSONObject_t*)malloc(capacity * sizeof(JSONObject_t)),
		.size = 0,
		.capacity = capacity
	};

	if (result.data)
	{
		memset(result.data, 0, capacity * sizeof(JSONObject_t));
	}

	return result;
}

WebFrameworkException appendJSONArrayObject(JSONArray_t* array, JSONObject_t* value)
{
	JSONObject_t object;
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

WebFrameworkException appendJSONArrayString(JSONArray_t* array, const char* value)
{
	JSONObject_t object;
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

WebFrameworkException appendJSONArrayInteger(JSONArray_t* array, int64_t value)
{
	JSONObject_t object;
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

WebFrameworkException appendJSONArrayUnsignedInteger(JSONArray_t* array, uint64_t value)
{
	JSONObject_t object;
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

WebFrameworkException appendJSONArrayDouble(JSONArray_t* array, double value)
{
	JSONObject_t object;
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

WebFrameworkException appendJSONArrayBoolean(JSONArray_t* array, bool value)
{
	JSONObject_t object;
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

WebFrameworkException appendJSONArrayNull(JSONArray_t* array)
{
	JSONObject_t object;
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

WebFrameworkException appendJSONArrayArray(JSONArray_t* array, const JSONArray_t* objects)
{
	WebFrameworkException exception = NULL;

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

void deleteJSONArray(JSONArray_t* array)
{
	for (size_t i = 0; i < array->size; i++)
	{
		deleteJSONObject(&array->data[i]);
	}

	free(array->data);
	array->capacity = 0;
	array->size = 0;
}

WebFrameworkException createJSONObject(JSONObject_t* jsonObject)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONObject)(void* _, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJSONObject, NULL, &exception);
	jsonObject->weak = false;

	return exception;
}

WebFrameworkException copyJSONObject(JSONObject_t* jsonObject, const JSONObject_t* other)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONObject)(void* other, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJSONObject, other->implementation, &exception);
	jsonObject->weak = false;

	return exception;
}

WebFrameworkException setJSONObjectObject(JSONObject_t* jsonObject, const char* key, JSONObject_t* object)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectObject)(void* implementation, const char* key, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectObject, key, object->implementation, &exception);

	return exception;
}

WebFrameworkException setJSONObjectString(JSONObject_t* jsonObject, const char* key, const char* value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectString)(void* implementation, const char* key, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectString, key, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectInteger(JSONObject_t* jsonObject, const char* key, int64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectInteger)(void* implementation, const char* key, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectInteger, key, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectUnsignedInteger(JSONObject_t* jsonObject, const char* key, uint64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectUnsignedInteger)(void* implementation, const char* key, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectUnsignedInteger, key, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectDouble(JSONObject_t* jsonObject, const char* key, double value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectDouble)(void* implementation, const char* key, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectDouble, key, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectBoolean(JSONObject_t* jsonObject, const char* key, bool value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectBoolean)(void* implementation, const char* key, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectBoolean, key, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectNull(JSONObject_t* jsonObject, const char* key)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectNull)(void* implementation, const char* key, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectNull, key, &exception);

	return exception;
}

WebFrameworkException setJSONObjectArray(JSONObject_t* jsonObject, const char* key, const JSONArray_t* array)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;
	void* buffer = malloc(array->size * sizeof(void*));
	void** value = &buffer;

	typedef void (*setJSONObjectArray)(void* implementation, const char* key, void** value, size_t size, void** exception);

	for (size_t i = 0; i < array->size; i++)
	{
		value[i] = array->data[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectArray, key, value, array->size, &exception);

	free(buffer);

	return exception;
}

void deleteJSONObject(JSONObject_t* jsonObject)
{
	typedef void* (*deleteWebFrameworkJSONObject)(void* implementation);

	if (!jsonObject->weak && jsonObject->implementation)
	{
		CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJSONObject, jsonObject->implementation);

		jsonObject->implementation = NULL;
	}
}
