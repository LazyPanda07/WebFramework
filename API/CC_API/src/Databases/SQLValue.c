#include "SQLValue.h"

WebFrameworkException createSQLValue(SQLValue* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*createSQLValue)(void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(createSQLValue, &exception);

	return exception;
}

WebFrameworkException setSQLValueInt(SQLValue implementation, int64_t value)
{
	WebFrameworkException exception = NULL;

	typedef void (*setSQLValueInt)(void* implementation, int64_t value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueInt, value, &exception);

	return exception;
}

WebFrameworkException setSQLValueDouble(SQLValue implementation, double value)
{
	WebFrameworkException exception = NULL;

	typedef void (*setSQLValueDouble)(void* implementation, double value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueDouble, value, &exception);

	return exception;
}

WebFrameworkException setSQLValueString(SQLValue implementation, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*setSQLValueString)(void* implementation, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueString, value, &exception);

	return exception;
}

WebFrameworkException setSQLValueBool(SQLValue implementation, bool value)
{
	WebFrameworkException exception = NULL;

	typedef void (*setSQLValueBool)(void* implementation, bool value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueBool, value, &exception);

	return exception;
}

WebFrameworkException setSQLValueNull(SQLValue implementation)
{
	WebFrameworkException exception = NULL;

	typedef void (*setSQLValueNull)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueNull, &exception);

	return exception;
}

WebFrameworkException setSQLValueBlob(SQLValue implementation, const uint8_t* value, size_t size)
{
	WebFrameworkException exception = NULL;

	typedef void (*setSQLValueBlob)(void* implementation, const uint8_t* value, size_t size, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setSQLValueBlob, value, size, &exception);

	return exception;
}

WebFrameworkException getSQLValueInt(SQLValue implementation, int64_t* result)
{
	WebFrameworkException exception = NULL;

	typedef int64_t(*getSQLValueInt)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueInt, &exception);

	return exception;
}

WebFrameworkException getSQLValueDouble(SQLValue implementation, double* result)
{
	WebFrameworkException exception = NULL;

	typedef double(*getSQLValueDouble)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueDouble, &exception);

	return exception;
}

WebFrameworkException getSQLValueString(SQLValue implementation, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getSQLValueString)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueString, &exception);

	return exception;
}

WebFrameworkException getSQLValueBool(SQLValue implementation, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool (*getSQLValueBool)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueBool, &exception);

	return exception;
}

WebFrameworkException getSQLValueBlob(SQLValue implementation, const uint8_t** result, size_t* size)
{
	WebFrameworkException exception = NULL;

	typedef const uint8_t* (*getSQLValueBlob)(void* implementation, size_t* size, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueBlob, size, &exception);

	return exception;
}

WebFrameworkException getSQLValueType(SQLValue implementation, SQLValueType* result)
{
	WebFrameworkException exception = NULL;

	typedef int (*getSQLValueType)(void* implementation, void** exception);

	*result = (SQLValueType)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLValueType, &exception);

	return exception;
}

void deleteWebFrameworkSQLValue(SQLValue implementation)
{
	typedef void* (*deleteWebFrameworkSQLValue)(void* implementation);

	CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkSQLValue, implementation);
}
