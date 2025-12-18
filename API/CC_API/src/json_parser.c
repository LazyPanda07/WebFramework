#include "json_parser.h"

static void __add_array_callback(void* object, void* array)
{
	json_object_t* jsonArray = (json_object_t*)array;
	json_object_t element;

	web_framework_exception_t exception = wf_emplace_json_object(jsonArray, &element);

	if (exception)
	{
		// TODO: error handling
	}

	wf_set_json_object_object(&element, (json_object_t*)object);
}

web_framework_exception_t wf_create_json_parser(json_parser_t* parser)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonParser)(void* _, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJsonParser, NULL, &exception);

	return exception;
}

web_framework_exception_t wf_copy_json_parser(json_parser_t* parser, const json_parser_t* other)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonParser)(const void* other, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJsonParser, other, &exception);

	return exception;
}

web_framework_exception_t wf_create_json_parser_from_string(const char* jsonString, json_parser_t* parser)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonParserFromString)(const char* jsonString, void** exception);

	*parser = CALL_WEB_FRAMEWORK_FUNCTION(createJsonParserFromString, jsonString, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_parser_object(json_parser_t implementation, const char* key, bool recursive, json_object_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getJsonParserObject)(void* implementation, const char* key, bool recursive, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserObject, key, recursive, &exception);
	result->weak = false;

	return exception;
}

web_framework_exception_t wf_get_json_parser_string(json_parser_t implementation, const char* key, bool recursive, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getJsonParserString)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserString, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_parser_integer(json_parser_t implementation, const char* key, bool recursive, int64_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int64_t(*getJsonParserInteger)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserInteger, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_parser_unsigned_integer(json_parser_t implementation, const char* key, bool recursive, uint64_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef uint64_t(*getJsonParserUnsignedInteger)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserUnsignedInteger, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_parser_double(json_parser_t implementation, const char* key, bool recursive, double* result)
{
	web_framework_exception_t exception = NULL;

	typedef double (*getJsonParserDouble)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserDouble, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_parser_boolean(json_parser_t implementation, const char* key, bool recursive, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*getJsonParserBoolean)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserBoolean, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_parser_null(json_parser_t implementation, const char* key, bool recursive, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*getJsonParserNull)(void* implementation, const char* key, bool recursive, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserNull, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_parser_array(json_parser_t implementation, const char* key, json_object_t* array, size_t* arraySize, bool recursive)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getJsonParserArray)(void* implementation, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserArray, key, __add_array_callback, array, recursive, &exception);

	if (exception)
	{
		return exception;
	}

	exception = wf_size_json_object(array, arraySize);

	return exception;
}

bool wf_try_get_json_parser_object(json_parser_t implementation, const char* key, json_object_t* value, bool recursive, web_framework_exception_t* exception)
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

bool wf_try_get_json_parser_string(json_parser_t implementation, const char* key, const char** value, bool recursive, web_framework_exception_t* exception)
{
	void* string = NULL;

	typedef bool (*tryGetJsonParserString)(void* implementation, const char* key, void** value, bool recursive, void** exception);

	if (CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserString, key, &string, recursive, exception))
	{
		*value = wf_get_data_from_string(string);

		wf_delete_string(string);

		return true;
	}

	return false;
}

bool wf_try_get_json_parser_integer(json_parser_t implementation, const char* key, int64_t* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserInteger)(void* implementation, const char* key, int64_t* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserInteger, key, value, recursive, exception);
}

bool wf_try_get_json_parser_unsigned_integer(json_parser_t implementation, const char* key, uint64_t* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserUnsignedInteger)(void* implementation, const char* key, uint64_t* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserUnsignedInteger, key, value, recursive, exception);
}

bool wf_try_get_json_parser_double(json_parser_t implementation, const char* key, double* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserDouble)(void* implementation, const char* key, double* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserDouble, key, value, recursive, exception);
}

bool wf_try_get_json_parser_boolean(json_parser_t implementation, const char* key, bool* value, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserBoolean)(void* implementation, const char* key, bool* value, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserBoolean, key, value, recursive, exception);
}

bool wf_try_get_json_parser_null(json_parser_t implementation, const char* key, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserNull)(void* implementation, const char* key, bool recursive, void** exception);

	return CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserNull, key, recursive, exception);
}

bool wf_try_get_json_parser_array(json_parser_t implementation, const char* key, json_object_t* array, size_t* arraySize, bool recursive, web_framework_exception_t* exception)
{
	typedef bool (*tryGetJsonParserArray)(void* implementation, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

	bool result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserArray, key, __add_array_callback, array, recursive, exception);

	if (*exception)
	{
		return false;
	}

	*exception = wf_size_json_object(array, arraySize);

	return result;
}
