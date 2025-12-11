#include "databases/sql_result.h"

web_framework_exception_t wf_get_sql_result_size(sql_result_t implementation, size_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef size_t(*getSQLResultSize)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLResultSize, &exception);

	return exception;
}

web_framework_exception_t wf_iterate_sql_result(sql_result_t implementation, void(*init_buffer)(size_t size, void* buffer), void(*callback)(const char** column_names, const sql_value_t* column_values, size_t size, size_t index, void* buffer), void* buffer)
{
	web_framework_exception_t exception = NULL;

	typedef void(*iterateSQLResult)(void* implementation, void(*init_buffer)(size_t size, void* buffer), void(*callback)(const char** column_names, const sql_value_t* column_values, size_t size, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(iterateSQLResult, init_buffer, callback, buffer, &exception);

	return exception;
}
