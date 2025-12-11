#include "databases/table.h"

web_framework_exception_t wf_execute_query(table_t implementation, const char* query, const sql_value_t* values, size_t size, sql_result_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*executeQuery)(void* implementation, const char* query, const void** values, size_t size, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(executeQuery, query, (const void**)values, size, &exception);

	return exception;
}

web_framework_exception_t wf_delete_sql_result(table_t implementation, sql_result_t result)
{
	web_framework_exception_t exception = NULL;

	typedef void (*deleteSQLResult)(void* implementation, void* result, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(deleteSQLResult, result, &exception);

	return exception;
}

web_framework_exception_t wf_get_table_name(table_t implementation, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getTableName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTableName, &exception);

	return exception;
}
