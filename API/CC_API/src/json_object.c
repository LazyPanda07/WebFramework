#include "JsonObject.h"

WebFrameworkException createJsonObject(JsonObject_t* jsonObject)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJsonObject)(void* _, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, NULL, &exception);
	jsonObject->weak = false;

	return exception;
}

WebFrameworkException copyJsonObject(JsonObject_t* jsonObject, const JsonObject_t* other)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJsonObject)(void* other, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, other->implementation, &exception);
	jsonObject->weak = false;

	return exception;
}

WebFrameworkException setJsonObjectObject(JsonObject_t* jsonObject, JsonObject_t* object)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectObject)(void* implementation, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectObject, object->implementation, &exception);

	return exception;
}

WebFrameworkException setJsonObjectString(JsonObject_t* jsonObject, const char* value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectString)(void* implementation, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectString, value, &exception);

	return exception;
}

WebFrameworkException setJsonObjectInteger(JsonObject_t* jsonObject, int64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectInteger)(void* implementation, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectInteger, value, &exception);

	return exception;
}

WebFrameworkException setJsonObjectUnsignedInteger(JsonObject_t* jsonObject, uint64_t value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectUnsignedInteger)(void* implementation, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectUnsignedInteger, value, &exception);

	return exception;
}

WebFrameworkException setJsonObjectDouble(JsonObject_t* jsonObject, double value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectDouble)(void* implementation, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectDouble, value, &exception);

	return exception;
}

WebFrameworkException setJsonObjectBoolean(JsonObject_t* jsonObject, bool value)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectBoolean)(void* implementation, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectBoolean, value, &exception);

	return exception;
}

WebFrameworkException setJsonObjectNull(JsonObject_t* jsonObject)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectNull)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectNull, &exception);

	return exception;
}

WebFrameworkException setJsonObjectArray(JsonObject_t* jsonObject, const JsonObject_t* array, size_t arraySize)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;
	void* buffer = malloc(arraySize * sizeof(void*));
	void** value = &buffer;

	typedef void (*setJsonObjectArray)(void* implementation, void** value, size_t size, void** exception);

	for (size_t i = 0; i < arraySize; i++)
	{
		value[i] = array[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectArray, value, arraySize, &exception);

	free(buffer);

	return exception;
}

WebFrameworkException assignJsonObject(JsonObject_t* jsonObject, const char* key, JsonObject_t* result)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void* (*accessKeyOperatorJsonObject)(void* implementation, const char* key, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonObject, key, &exception);
	result->weak = true;

	return exception;
}

WebFrameworkException emplaceJsonObject(JsonObject_t* jsonObject, JsonObject_t* result)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void* (*emplaceBackNull)(void* implementation, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackNull, &exception);
	result->weak = true;

	return exception;
}

WebFrameworkException sizeJsonObject(JsonObject_t* jsonObject, size_t* size)
{
	WebFrameworkException exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef size_t (*sizeJsonObject)(void* implementation, void** exception);

	*size = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sizeJsonObject, &exception);

	return exception;
}

void deleteJsonObject(JsonObject_t* jsonObject)
{
	typedef void* (*deleteWebFrameworkJsonObject)(void* implementation);

	if (jsonObject && !jsonObject->weak && jsonObject->implementation)
	{
		CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonObject, jsonObject->implementation);

		jsonObject->implementation = NULL;
	}
}
