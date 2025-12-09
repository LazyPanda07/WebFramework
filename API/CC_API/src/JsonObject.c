#include "JsonObject.h"

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

WebFrameworkException setJSONObjectArray(JSONObject_t* jsonObject, const JSONObject_t* array, size_t arraySize)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;
	void* buffer = malloc(arraySize * sizeof(void*));
	void** value = &buffer;

	typedef void (*setJSONObjectArray)(void* implementation, void** value, size_t size, void** exception);

	for (size_t i = 0; i < arraySize; i++)
	{
		value[i] = array[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectArray, value, arraySize, &exception);

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

WebFrameworkException sizeJSONObject(JSONObject_t* jsonObject, size_t* size)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef size_t (*sizeJSONObject)(void* implementation, void** exception);

	*size = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sizeJSONObject, &exception);

	return exception;
}

void deleteJSONObject(JSONObject_t* jsonObject)
{
	typedef void* (*deleteWebFrameworkJSONObject)(void* implementation);

	if (jsonObject && !jsonObject->weak && jsonObject->implementation)
	{
		CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJSONObject, jsonObject->implementation);

		jsonObject->implementation = NULL;
	}
}
