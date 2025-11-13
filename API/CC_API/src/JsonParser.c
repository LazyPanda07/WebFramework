#include "JsonParser.h"

static void __appendJSONArrayObject(JSONArray_t* array, void* value)
{
	size_t index = __getIndex(array);

	array->data[index].implementation = value;
	array->data[index].weak = true;
}

static void __addArrayCallback(void* object, void* array)
{
	JSONArray_t* jsonArray = (JSONArray_t*)array;

	__appendJSONArrayObject(jsonArray, object);
}

WebFrameworkException createJSONParser(JSONParser* parser)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONParser)(void* _, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJSONParser, NULL, &exception);

	return exception;
}

WebFrameworkException copyJSONParser(JSONParser* parser, const JSONParser* other)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONParser)(const void* other, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJSONParser, other, &exception);

	return exception;
}

WebFrameworkException createJSONParserFromString(const char* jsonString, JSONParser* parser)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createJSONParserFromString)(const char* jsonString, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJSONParserFromString, jsonString, &exception);

	return exception;
}

WebFrameworkException getJSONParserObject(JSONParser implementation, const char* key, bool recursive, JSONObject_t* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getJSONParserObject)(void* implementation, const char* key, bool recursive, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserObject, key, recursive, &exception);
	result->weak = false;

	return exception;
}

WebFrameworkException getJSONParserString(JSONParser implementation, const char* key, bool recursive, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getJSONParserString)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserString, key, recursive, &exception);

	return exception;
}

WebFrameworkException getJSONParserInteger(JSONParser implementation, const char* key, bool recursive, int64_t* result)
{
	WebFrameworkException exception = NULL;

	typedef int64_t(*getJSONParserInteger)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserInteger, key, recursive, &exception);

	return exception;
}

WebFrameworkException getJSONParserUnsignedInteger(JSONParser implementation, const char* key, bool recursive, uint64_t* result)
{
	WebFrameworkException exception = NULL;

	typedef uint64_t(*getJSONParserUnsignedInteger)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserUnsignedInteger, key, recursive, &exception);

	return exception;
}

WebFrameworkException getJSONParserDouble(JSONParser implementation, const char* key, bool recursive, double* result)
{
	WebFrameworkException exception = NULL;

	typedef double (*getJSONParserDouble)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserDouble, key, recursive, &exception);

	return exception;
}

WebFrameworkException getJSONParserBoolean(JSONParser implementation, const char* key, bool recursive, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool (*getJSONParserBoolean)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserBoolean, key, recursive, &exception);

	return exception;
}

WebFrameworkException getJSONParserNull(JSONParser implementation, const char* key, bool recursive, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool (*getJSONParserNull)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserNull, key, recursive, &exception);

	return exception;
}

WebFrameworkException getJSONParserArray(JSONParser implementation, const char* key, JSONArray_t* array, bool recursive)
{
	WebFrameworkException exception = NULL;

	typedef void (*getJSONParserArray)(void* implementation, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserArray, key, __addArrayCallback, array, recursive, &exception);

	return exception;
}

bool tryGetJSONParserObject(JSONParser implementation, const char* key, JSONObject_t* value, bool recursive, WebFrameworkException* exception)
{
	void* object = NULL;

	typedef bool (*tryGetJSONParserObject)(void* implementation, const char* key, void** value, bool recursive, void** exception);

	if (CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserObject, key, &object, recursive, exception))
	{
		value->implementation = object;
		value->weak = true;

		return true;
	}

	return false;
}

bool tryGetJSONParserString(JSONParser implementation, const char* key, const char** value, bool recursive, WebFrameworkException* exception)
{
	void* string = NULL;

	typedef bool (*tryGetJSONParserString)(void* implementation, const char* key, void** value, bool recursive, void** exception);

	if (CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserString, key, &string, recursive, exception))
	{
		*value = getDataFromString(string);

		deleteWebFrameworkString(string);

		return true;
	}

	return false;
}

bool tryGetJSONParserInteger(JSONParser implementation, const char* key, int64_t* value, bool recursive, WebFrameworkException* exception)
{
	typedef bool (*tryGetJSONParserInteger)(void* implementation, const char* key, int64_t* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserInteger, key, value, recursive, exception);
}

bool tryGetJSONParserUnsignedInteger(JSONParser implementation, const char* key, uint64_t* value, bool recursive, WebFrameworkException* exception)
{
	typedef bool (*tryGetJSONParserUnsignedInteger)(void* implementation, const char* key, uint64_t* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserUnsignedInteger, key, value, recursive, exception);
}

bool tryGetJSONParserDouble(JSONParser implementation, const char* key, double* value, bool recursive, WebFrameworkException* exception)
{
	typedef bool (*tryGetJSONParserDouble)(void* implementation, const char* key, double* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserDouble, key, value, recursive, exception);
}

bool tryGetJSONParserBoolean(JSONParser implementation, const char* key, bool* value, bool recursive, WebFrameworkException* exception)
{
	typedef bool (*tryGetJSONParserBoolean)(void* implementation, const char* key, bool* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserBoolean, key, value, recursive, exception);
}

bool tryGetJSONParserNull(JSONParser implementation, const char* key, bool recursive, WebFrameworkException* exception)
{
	typedef bool (*tryGetJSONParserNull)(void* implementation, const char* key, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserNull, key, recursive, exception);
}

bool tryGetJSONParserArray(JSONParser implementation, const char* key, JSONArray_t* array, bool recursive, WebFrameworkException* exception)
{
	typedef bool (*tryGetJSONParserArray)(void* implementation, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserArray, key, __addArrayCallback, array, recursive, exception);
}
