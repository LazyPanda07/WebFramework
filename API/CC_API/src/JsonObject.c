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

	exception = setJSONObjectObject(&object, value);

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

	exception = setJSONObjectString(&object, value);

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

	exception = setJSONObjectInteger(&object, value);

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

	exception = setJSONObjectUnsignedInteger(&object, value);

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

	exception = setJSONObjectDouble(&object, value);

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

	exception = setJSONObjectBoolean(&object, value);

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

	exception = setJSONObjectNull(&object);

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

WebFrameworkException setJSONObjectObject(JSONObject_t* jsonObject, JSONObject_t* object)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectObject)(void* implementation, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectObject, object->implementation, &exception);

	return exception;
}

WebFrameworkException setJSONObjectString(JSONObject_t* jsonObject, const char* value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectString)(void* implementation, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectString, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectInteger(JSONObject_t* jsonObject, int64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectInteger)(void* implementation, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectInteger, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectUnsignedInteger(JSONObject_t* jsonObject, uint64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectUnsignedInteger)(void* implementation, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectUnsignedInteger, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectDouble(JSONObject_t* jsonObject, double value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectDouble)(void* implementation, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectDouble, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectBoolean(JSONObject_t* jsonObject, bool value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectBoolean)(void* implementation, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectBoolean, value, &exception);

	return exception;
}

WebFrameworkException setJSONObjectNull(JSONObject_t* jsonObject)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJSONObjectNull)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectNull, &exception);

	return exception;
}

WebFrameworkException setJSONObjectArray(JSONObject_t* jsonObject, const JSONArray_t* array)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;
	void* buffer = malloc(array->size * sizeof(void*));
	void** value = &buffer;

	typedef void (*setJSONObjectArray)(void* implementation, void** value, size_t size, void** exception);

	for (size_t i = 0; i < array->size; i++)
	{
		value[i] = array->data[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectArray, value, array->size, &exception);

	free(buffer);

	return exception;
}

WebFrameworkException assignJSONObject(JSONObject_t* jsonObject, const char* key, JSONObject_t* result)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void* (*accessKeyOperatorJsonObject)(void* implementation, const char* key, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonObject, key, &exception);
	result->weak = true;

	return exception;
}

WebFrameworkException emplaceJSONObject(JSONObject_t* jsonObject, JSONObject_t* result)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void* (*emplaceBackNull)(void* implementation, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackNull, &exception);
	result->weak = true;

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
