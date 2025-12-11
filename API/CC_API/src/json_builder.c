#include "JsonBuilder.h"

WebFrameworkException createJsonBuilder(JsonBuilder* builder)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJsonBuilder)(void* _, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJsonBuilder, NULL, &exception);

	return exception;
}

WebFrameworkException copyJsonBuilder(JsonBuilder* builder, JsonBuilder other)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJsonBuilder)(void* other, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJsonBuilder, other, &exception);

	return exception;
}

WebFrameworkException buildJsonBuilder(JsonBuilder implementation, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*buildJsonBuilder)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(buildJsonBuilder, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderObject(JsonBuilder implementation, const char* key, JsonObject_t* object)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJsonBuilderObject)(void* implementation, const char* key, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderObject, key, object->implementation, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderString(JsonBuilder implementation, const char* key, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJsonBuilderString)(void* implementation, const char* key, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderString, key, value, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderInteger(JsonBuilder implementation, const char* key, int64_t value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJsonBuilderInteger)(void* implementation, const char* key, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderInteger, key, value, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderUnsignedInteger(JsonBuilder implementation, const char* key, uint64_t value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJsonBuilderUnsignedInteger)(void* implementation, const char* key, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderUnsignedInteger, key, value, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderDouble(JsonBuilder implementation, const char* key, double value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJsonBuilderDouble)(void* implementation, const char* key, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderDouble, key, value, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderBoolean(JsonBuilder implementation, const char* key, bool value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJsonBuilderBoolean)(void* implementation, const char* key, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderBoolean, key, value, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderNull(JsonBuilder implementation, const char* key)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJsonBuilderBoolean)(void* implementation, const char* key, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderBoolean, key, &exception);

	return exception;
}

WebFrameworkException appendJsonBuilderArray(JsonBuilder implementation, const char* key, const JsonObject_t* array, size_t arraySize)
{
	WebFrameworkException exception = NULL;
	void* buffer = malloc(arraySize * sizeof(void*));

	if (!buffer)
	{
		return exception;
	}

	void** value = (void**)buffer;

	typedef void (*appendJsonBuilderArray)(void* implementation, const char* key, void** value, size_t size, void** exception);

	for (size_t i = 0; i < arraySize; i++)
	{
		value[i] = array[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderArray, key, value, arraySize, &exception);

	free(buffer);

	return exception;
}
