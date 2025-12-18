#include "json_object.h"

web_framework_exception_t wf_create_json_object(json_object_t* jsonObject)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonObject)(void* _, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, NULL, &exception);
	jsonObject->weak = false;

	return exception;
}

web_framework_exception_t wf_copy_json_object(json_object_t* jsonObject, const json_object_t* other)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJsonObject)(void* other, void** exception);

	jsonObject->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, other->implementation, &exception);
	jsonObject->weak = false;

	return exception;
}

web_framework_exception_t wf_set_json_object_object(json_object_t* jsonObject, json_object_t* object)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectObject)(void* implementation, void* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectObject, object->implementation, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_string(json_object_t* jsonObject, const char* value)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectString)(void* implementation, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectString, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_integer(json_object_t* jsonObject, int64_t value)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectInteger)(void* implementation, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectInteger, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_unsigned_integer(json_object_t* jsonObject, uint64_t value)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectUnsignedInteger)(void* implementation, uint64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectUnsignedInteger, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_double(json_object_t* jsonObject, double value)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectDouble)(void* implementation, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectDouble, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_boolean(json_object_t* jsonObject, bool value)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectBoolean)(void* implementation, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectBoolean, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_null(json_object_t* jsonObject)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void (*setJsonObjectNull)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectNull, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_object_array(json_object_t* jsonObject, const json_object_t* array, size_t arraySize)
{
	web_framework_exception_t exception = NULL;
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

web_framework_exception_t wf_assign_json_object(json_object_t* jsonObject, const char* key, json_object_t* result)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void* (*accessKeyOperatorJsonObject)(void* implementation, const char* key, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonObject, key, &exception);
	result->weak = true;

	return exception;
}

web_framework_exception_t wf_emplace_json_object(json_object_t* jsonObject, json_object_t* result)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef void* (*emplaceBackNull)(void* implementation, void** exception);

	result->implementation = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackNull, &exception);
	result->weak = true;

	return exception;
}

web_framework_exception_t wf_size_json_object(json_object_t* jsonObject, size_t* size)
{
	web_framework_exception_t exception = NULL;
	void* implementation = jsonObject->implementation;

	typedef size_t (*sizeJsonObject)(void* implementation, void** exception);

	*size = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sizeJsonObject, &exception);

	return exception;
}

void wf_delete_json_object(json_object_t* jsonObject)
{
	typedef void* (*deleteWebFrameworkJsonObject)(void* implementation);

	if (jsonObject && !jsonObject->weak && jsonObject->implementation)
	{
		CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonObject, jsonObject->implementation);

		jsonObject->implementation = NULL;
	}
}
