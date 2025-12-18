#include "databases/sql_value.h"

web_framework_exception_t wf_create_sql_value(sql_value_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createSQLValue)(void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(createSQLValue, &exception);

	return exception;
}

web_framework_exception_t wf_set_sql_value_int(sql_value_t implementation, int64_t value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setSQLValueInt)(void* implementation, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueInt, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_sql_value_double(sql_value_t implementation, double value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setSQLValueDouble)(void* implementation, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueDouble, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_sql_value_string(sql_value_t implementation, const char* value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setSQLValueString)(void* implementation, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueString, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_sql_value_bool(sql_value_t implementation, bool value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setSQLValueBool)(void* implementation, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueBool, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_sql_value_null(sql_value_t implementation)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setSQLValueNull)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueNull, &exception);

	return exception;
}

web_framework_exception_t wf_set_sql_value_blob(sql_value_t implementation, const uint8_t* value, size_t size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setSQLValueBlob)(void* implementation, const uint8_t* value, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueBlob, value, size, &exception);

	return exception;
}

web_framework_exception_t wf_get_sql_value_int(sql_value_t implementation, int64_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int64_t(*getSQLValueInt)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueInt, &exception);

	return exception;
}

web_framework_exception_t wf_get_sql_value_double(sql_value_t implementation, double* result)
{
	web_framework_exception_t exception = NULL;

	typedef double(*getSQLValueDouble)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueDouble, &exception);

	return exception;
}

web_framework_exception_t wf_get_sql_value_string(sql_value_t implementation, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getSQLValueString)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueString, &exception);

	return exception;
}

web_framework_exception_t wf_get_sql_value_bool(sql_value_t implementation, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*getSQLValueBool)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueBool, &exception);

	return exception;
}

web_framework_exception_t wf_get_sql_value_blob(sql_value_t implementation, const uint8_t** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef const uint8_t* (*getSQLValueBlob)(void* implementation, size_t* size, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueBlob, size, &exception);

	return exception;
}

web_framework_exception_t wf_get_sql_value_type(sql_value_t implementation, sql_value_type_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int (*getSQLValueType)(void* implementation, void** exception);

	*result = (sql_value_type_t)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueType, &exception);

	return exception;
}

void wf_delete_sql_value(sql_value_t implementation)
{
	typedef void* (*deleteWebFrameworkSQLValue)(void* implementation);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkSQLValue, implementation);
}
