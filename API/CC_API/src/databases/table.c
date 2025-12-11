#include "Databases/Table.h"

WebFrameworkException executeQuery(Table implementation, const char* query, const SQLValue* values, size_t size, SQLResult* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*executeQuery)(void* implementation, const char* query, const void** values, size_t size, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(executeQuery, query, (const void**)values, size, &exception);

	return exception;
}

WebFrameworkException deleteSQLResult(Table implementation, SQLResult result)
{
	WebFrameworkException exception = NULL;

	typedef void (*deleteSQLResult)(void* implementation, void* result, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(deleteSQLResult, result, &exception);

	return exception;
}

WebFrameworkException getTableName(Table implementation, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getTableName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTableName, &exception);

	return exception;
}
