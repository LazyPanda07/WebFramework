#include "json_parser_t.h"

static void __addArrayCallback(void* object, void* array)
{
	JsonObject_t* jsonArray = (JsonObject_t*)array;
	JsonObject_t element;

	web_framework_exception_t exception = emplaceJsonObject(jsonArray, &element);

	if (exception)
	{
		// TODO: error handling
	}

	setJsonObjectObject(&element, (JsonObject_t*)object);
}

web_framework_exception_t createJsonParser(json_parser_t* parser)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonParser)(void* _, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJsonParser, NULL, &exception);

	return exception;
}

web_framework_exception_t copyJsonParser(json_parser_t* parser, const json_parser_t* other)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonParser)(const void* other, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJsonParser, other, &exception);

	return exception;
}

web_framework_exception_t createJsonParserFromString(const char* jsonString, json_parser_t* parser)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonParserFromString)(const char* jsonString, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJsonParserFromString, jsonString, &exception);

	return exception;
}

web_framework_exception_t getJsonParserObject(json_parser_t implementation, const char* key, bool recursive, JsonObject_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getJsonParserObject)(void* implementation, const char* key, bool recursive, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserObject, key, recursive, &exception);
	result->weak = false;

	return exception;
}

web_framework_exception_t getJsonParserString(json_parser_t implementation, const char* key, bool recursive, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getJsonParserString)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserString, key, recursive, &exception);

	return exception;
}

web_framework_exception_t getJsonParserInteger(json_parser_t implementation, const char* key, bool recursive, int64_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int64_t(*getJsonParserInteger)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserInteger, key, recursive, &exception);

	return exception;
}

web_framework_exception_t getJsonParserUnsignedInteger(json_parser_t implementation, const char* key, bool recursive, uint64_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef uint64_t(*getJsonParserUnsignedInteger)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserUnsignedInteger, key, recursive, &exception);

	return exception;
}

web_framework_exception_t getJsonParserDouble(json_parser_t implementation, const char* key, bool recursive, double* result)
{
	web_framework_exception_t exception = NULL;

	typedef double (*getJsonParserDouble)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserDouble, key, recursive, &exception);

	return exception;
}

web_framework_exception_t getJsonParserBoolean(json_parser_t implementation, const char* key, bool recursive, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*getJsonParserBoolean)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserBoolean, key, recursive, &exception);

	return exception;
}

web_framework_exception_t getJsonParserNull(json_parser_t implementation, const char* key, bool recursive, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*getJsonParserNull)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserNull, key, recursive, &exception);

	return exception;
}

web_framework_exception_t getJsonParserArray(json_parser_t implementation, const char* key, JsonObject_t* array, size_t* arraySize, bool recursive)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getJsonParserArray)(void* implementation, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserArray, key, __addArrayCallback, array, recursive, &exception);

	if (exception)
	{
		return exception;
	}

	exception = sizeJsonObject(array, arraySize);

	return exception;
}

bool tryGetJsonParserObject(json_parser_t implementation, const char* key, JsonObject_t* value, bool recursive, web_framework_exception_t* exception)
{
	void* object = NULL;

	typedef bool (*tryGetJsonParserObject)(void* implementation, const char* key, void** value, bool recursive, void** exception);

	if (CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserObject, key, &object, recursive, exception))
	{
		value->implementation = object;
		value->weak = true;

		return true;
	}

	return false;
}

bool tryGetJsonParserString(json_parser_t implementation, const char* key, const char** value, bool recursive, web_framework_exception_t* exception)
{
	void* string = NULL;

	typedef bool (*tryGetJsonParserString)(void* implementation, const char* key, void** value, bool recursive, void** exception);

	if (CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserString, key, &string, recursive, exception))
	{
		*value = getDataFromString(string);

		deleteWebFrameworkString(string);

		return true;
	}

	return false;
}

bool tryGetJsonParserInteger(json_parser_t implementation, const char* key, int64_t* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserInteger)(void* implementation, const char* key, int64_t* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserInteger, key, value, recursive, exception);
}

bool tryGetJsonParserUnsignedInteger(json_parser_t implementation, const char* key, uint64_t* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserUnsignedInteger)(void* implementation, const char* key, uint64_t* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserUnsignedInteger, key, value, recursive, exception);
}

bool tryGetJsonParserDouble(json_parser_t implementation, const char* key, double* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserDouble)(void* implementation, const char* key, double* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserDouble, key, value, recursive, exception);
}

bool tryGetJsonParserBoolean(json_parser_t implementation, const char* key, bool* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserBoolean)(void* implementation, const char* key, bool* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserBoolean, key, value, recursive, exception);
}

bool tryGetJsonParserNull(json_parser_t implementation, const char* key, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserNull)(void* implementation, const char* key, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserNull, key, recursive, exception);
}

bool tryGetJsonParserArray(json_parser_t implementation, const char* key, JsonObject_t* array, size_t* arraySize, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserArray)(void* implementation, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

	bool result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserArray, key, __addArrayCallback, array, recursive, exception);

	if (*exception)
	{
		return false;
	}

	*exception = sizeJsonObject(array, arraySize);

	return result;
}
