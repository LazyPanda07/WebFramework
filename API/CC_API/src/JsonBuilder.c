#include "JsonBuilder.h"

WebFrameworkException createJSONBuilder(JSONBuilder* builder)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONBuilder)(void* _, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilder, NULL, &exception);

	return exception;
}

WebFrameworkException copyJSONBuilder(JSONBuilder* builder, JSONBuilder other)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONBuilder)(void* other, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilder, other, &exception);

	return exception;
}

WebFrameworkException buildJSONBuilder(JSONBuilder implementation, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*buildJSONBuilder)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(buildJSONBuilder, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderObject(JSONBuilder implementation, const char* key, JSONObject_t* object)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderObject)(void* implementation, const char* key, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderObject, key, object->implementation, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderString(JSONBuilder implementation, const char* key, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderString)(void* implementation, const char* key, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderString, key, value, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderInteger(JSONBuilder implementation, const char* key, int64_t value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderInteger)(void* implementation, const char* key, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderInteger, key, value, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderUnsignedInteger(JSONBuilder implementation, const char* key, uint64_t value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderUnsignedInteger)(void* implementation, const char* key, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderUnsignedInteger, key, value, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderDouble(JSONBuilder implementation, const char* key, double value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderDouble)(void* implementation, const char* key, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderDouble, key, value, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderBoolean(JSONBuilder implementation, const char* key, bool value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderBoolean)(void* implementation, const char* key, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderBoolean, key, value, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderNull(JSONBuilder implementation, const char* key)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderBoolean)(void* implementation, const char* key, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderBoolean, key, &exception);

	return exception;
}

WebFrameworkException appendJSONBuilderArray(JSONBuilder implementation, const char* key, const JSONObject_t* array, size_t arraySize)
{
	WebFrameworkException exception = NULL;
	void* buffer = malloc(arraySize * sizeof(void*));

	if (!buffer)
	{
		return exception;
	}

	void** value = (void**)buffer;

	typedef void (*appendJSONBuilderArray)(void* implementation, const char* key, void** value, size_t size, void** exception);

	for (size_t i = 0; i < arraySize; i++)
	{
		value[i] = array[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderArray, key, value, arraySize, &exception);

	free(buffer);

	return exception;
}
