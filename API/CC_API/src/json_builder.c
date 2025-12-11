#include "json_builder_t.h"

web_framework_exception_t createJsonBuilder(json_builder_t* builder)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonBuilder)(void* _, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJsonBuilder, NULL, &exception);

	return exception;
}

web_framework_exception_t copyJsonBuilder(json_builder_t* builder, json_builder_t other)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonBuilder)(void* other, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJsonBuilder, other, &exception);

	return exception;
}

web_framework_exception_t buildJsonBuilder(json_builder_t implementation, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*buildJsonBuilder)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(buildJsonBuilder, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderObject(json_builder_t implementation, const char* key, JsonObject_t* object)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*appendJsonBuilderObject)(void* implementation, const char* key, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderObject, key, object->implementation, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderString(json_builder_t implementation, const char* key, const char* value)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*appendJsonBuilderString)(void* implementation, const char* key, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderString, key, value, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderInteger(json_builder_t implementation, const char* key, int64_t value)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*appendJsonBuilderInteger)(void* implementation, const char* key, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderInteger, key, value, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderUnsignedInteger(json_builder_t implementation, const char* key, uint64_t value)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*appendJsonBuilderUnsignedInteger)(void* implementation, const char* key, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderUnsignedInteger, key, value, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderDouble(json_builder_t implementation, const char* key, double value)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*appendJsonBuilderDouble)(void* implementation, const char* key, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderDouble, key, value, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderBoolean(json_builder_t implementation, const char* key, bool value)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*appendJsonBuilderBoolean)(void* implementation, const char* key, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderBoolean, key, value, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderNull(json_builder_t implementation, const char* key)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*appendJsonBuilderBoolean)(void* implementation, const char* key, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderBoolean, key, &exception);

	return exception;
}

web_framework_exception_t appendJsonBuilderArray(json_builder_t implementation, const char* key, const JsonObject_t* array, size_t arraySize)
{
	web_framework_exception_t exception = NULL;
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
