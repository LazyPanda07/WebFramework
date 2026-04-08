#include "json_object.h"

static void __add_array_callback(void* object, void* array)
{
	json_object_t* jsonArray = (json_object_t*)array;
	json_object_t element;

	web_framework_exception_t exception = wf_emplace_json_object(jsonArray, &element);

	if (exception)
	{
		fprintf(stderr, "Can't add element to JSON array\n");

		exit(0xff);
	}

	wf_set_json_object_object(&element, (json_object_t*)object);
}

web_framework_exception_t wf_create_json_object(json_object_t* json_object)
{
	typedef void* (*createJsonObject)(void* _, void** exception);

	web_framework_exception_t exception = NULL;

	json_object->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, NULL, &exception);
	json_object->weak = false;

	return exception;
}

web_framework_exception_t wf_copy_json_object(json_object_t* json_object, const json_object_t* other)
{
	typedef void* (*createJsonObject)(void* other, void** exception);

	web_framework_exception_t exception = NULL;

	json_object->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, other->implementation, &exception);
	json_object->weak = false;

	return exception;
}

web_framework_exception_t wf_set_json_object_object(json_object_t* json_object, json_object_t* object)
{
	typedef void (*setJsonObjectObject)(void* implementation, void* value, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectObject, object->implementation, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_string(json_object_t* json_object, const char* value)
{
	typedef void (*setJsonObjectString)(void* implementation, const char* value, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectString, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_integer(json_object_t* json_object, int64_t value)
{
	typedef void (*setJsonObjectInteger)(void* implementation, int64_t value, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectInteger, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_unsigned_integer(json_object_t* json_object, uint64_t value)
{
	typedef void (*setJsonObjectUnsignedInteger)(void* implementation, uint64_t value, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectUnsignedInteger, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_double(json_object_t* json_object, double value)
{
	typedef void (*setJsonObjectDouble)(void* implementation, double value, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectDouble, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_boolean(json_object_t* json_object, bool value)
{
	typedef void (*setJsonObjectBoolean)(void* implementation, bool value, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectBoolean, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_null(json_object_t* json_object)
{
	typedef void (*setJsonObjectNull)(void* implementation, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectNull, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_array(json_object_t* json_object, const json_object_t* array, size_t arraySize)
{
	typedef void (*setJsonObjectArray)(void* implementation, void** value, size_t size, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;
	void* buffer = malloc(arraySize * sizeof(void*));
	void** value = &buffer;

	for (size_t i = 0; i < arraySize; i++)
	{
		value[i] = array[i].implementation;
	}

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectArray, value, arraySize, &exception);

	free(buffer);

	return exception;
}

web_framework_exception_t wf_get_json_object_object(json_object_t* json_object, const char* key, bool recursive, json_object_t* result)
{
	typedef void* (*getJsonObjectObject)(void* implementation, const char* key, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectObject, key, recursive, &exception);
	result->weak = false;

	return exception;
}

web_framework_exception_t wf_get_json_object_string(json_object_t* json_object, const char* key, bool recursive, const char** result)
{
	typedef const char* (*getJsonObjectString)(void* implementation, const char* key, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectString, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_object_integer(json_object_t* json_object, const char* key, bool recursive, int64_t* result)
{
	typedef int64_t(*getJsonObjectInteger)(void* implementation, const char* key, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectInteger, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_object_unsigned_integer(json_object_t* json_object, const char* key, bool recursive, uint64_t* result)
{
	typedef uint64_t(*getJsonObjectUnsignedInteger)(void* implementation, const char* key, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectUnsignedInteger, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_object_double(json_object_t* json_object, const char* key, bool recursive, double* result)
{
	typedef double (*getJsonObjectDouble)(void* implementation, const char* key, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectDouble, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_object_boolean(json_object_t* json_object, const char* key, bool recursive, bool* result)
{
	typedef bool (*getJsonObjectBoolean)(void* implementation, const char* key, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectBoolean, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_object_null(json_object_t* json_object, const char* key, bool recursive, bool* result)
{
	typedef bool (*getJsonObjectNull)(void* implementation, const char* key, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectNull, key, recursive, &exception);

	return exception;
}

web_framework_exception_t wf_get_json_object_array(json_object_t* json_object, const char* key, json_object_t* array, size_t* arraySize, bool recursive)
{
	typedef void (*getJsonObjectArray)(void* implementation, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectArray, key, __add_array_callback, array, recursive, &exception);

	if (exception)
	{
		return exception;
	}

	exception = wf_size_json_object(array, arraySize);

	return exception;
}

web_framework_exception_t wf_assign_json_object(json_object_t* json_object, const char* key, json_object_t* result)
{
	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	typedef void* (*accessKeyOperatorJsonObject)(void* implementation, const char* key, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonObject, key, &exception);
	result->weak = true;

	return exception;
}

web_framework_exception_t wf_emplace_json_object(json_object_t* json_object, json_object_t* result)
{
	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	typedef void* (*emplaceBackNull)(void* implementation, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackNull, &exception);
	result->weak = true;

	return exception;
}

web_framework_exception_t wf_size_json_object(json_object_t* json_object, size_t* size)
{
	web_framework_exception_t exception = NULL;
	void* implementation = json_object->implementation;

	typedef size_t(*sizeJsonObject)(void* implementation, void** exception);

	*size = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sizeJsonObject, &exception);

	return exception;
}

void wf_delete_json_object(json_object_t* json_object)
{
	typedef void* (*deleteWebFrameworkJsonObject)(void* implementation);

	if (json_object && !json_object->weak && json_object->implementation)
	{
		CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonObject, json_object->implementation);

		json_object->implementation = NULL;
	}
}
