#pragma once

#include "JSONObject.h"

WebFrameworkException createJSONBuilder(JSONBuilder* builder);

WebFrameworkException copyJSONBuilder(JSONBuilder* builder, JSONBuilder other);

WebFrameworkException buildJSONBuilder(JSONBuilder implementation, WebFrameworkString* result);

WebFrameworkException appendJSONBuilderObject(JSONBuilder implementation, const char* key, JSONObject* object);

WebFrameworkException appendJSONBuilderString(JSONBuilder implementation, const char* key, const char* value);

WebFrameworkException appendJSONBuilderInteger(JSONBuilder implementation, const char* key, int64_t value);

WebFrameworkException appendJSONBuilderUnsignedInteger(JSONBuilder implementation, const char* key, uint64_t value);

WebFrameworkException appendJSONBuilderDouble(JSONBuilder implementation, const char* key, double value);

WebFrameworkException appendJSONBuilderBoolean(JSONBuilder implementation, const char* key, bool value);

WebFrameworkException appendJSONBuilderNull(JSONBuilder implementation, const char* key);

WebFrameworkException appendJSONBuilderArray(JSONBuilder implementation, const char* key, const JSONArray* array);

inline WebFrameworkException createJSONBuilder(JSONBuilder* builder)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONBuilder)(void* _, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilder, NULL, &exception);

	return exception;
}

inline WebFrameworkException copyJSONBuilder(JSONBuilder* builder, JSONBuilder other)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONBuilder)(void* other, void** exception);

	*builder = CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilder, other, &exception);

	return exception;
}

inline WebFrameworkException buildJSONBuilder(JSONBuilder implementation, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*buildJSONBuilder)(void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(buildJSONBuilder, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderObject(JSONBuilder implementation, const char* key, JSONObject* object)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderObject)(void* implementation, const char* key, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderObject, key, object->implementation, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderString(JSONBuilder implementation, const char* key, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderString)(void* implementation, const char* key, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderString, key, value, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderInteger(JSONBuilder implementation, const char* key, int64_t value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderInteger)(void* implementation, const char* key, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderInteger, key, value, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderUnsignedInteger(JSONBuilder implementation, const char* key, uint64_t value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderUnsignedInteger)(void* implementation, const char* key, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderUnsignedInteger, key, value, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderDouble(JSONBuilder implementation, const char* key, double value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderDouble)(void* implementation, const char* key, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderDouble, key, value, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderBoolean(JSONBuilder implementation, const char* key, bool value)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderBoolean)(void* implementation, const char* key, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderBoolean, key, value, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderNull(JSONBuilder implementation, const char* key)
{
	WebFrameworkException exception = NULL;

	typedef void* (*appendJSONBuilderBoolean)(void* implementation, const char* key, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderBoolean, key, &exception);

	return exception;
}

inline WebFrameworkException appendJSONBuilderArray(JSONBuilder implementation, const char* key, const JSONArray* array)
{
	WebFrameworkException exception = NULL;
	void* buffer = malloc(array->size * sizeof(void*));
	void** value = &buffer;

	typedef void (*appendJSONBuilderArray)(void* implementation, const char* key, void** value, size_t size, void** exception);

	for (size_t i = 0; i < array->size; i++)
	{
		value[i] = array->data[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderArray, key, value, array->size, &exception);

	free(buffer);

	return exception;
}
