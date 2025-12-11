#include "Databases/database_t.h"

web_framework_exception_t wf_get_or_create_table(database_t implementation, const char* table_name, const char* create_table_query, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getOrCreateTable)(void* implementation, const char* table_name, const char* create_table_query, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateTable, table_name, create_table_query, &exception);

	return exception;
}

web_framework_exception_t wf_get_table(database_t implementation, const char* table_name, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getTable)(void* implementation, const char* table_name, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTable, table_name, &exception);

	return exception;
}

web_framework_exception_t wf_contains_table(database_t implementation, const char* table_name, table_t* out_table, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*containsTable)(void* implementation, const char* table_name, void** out_table, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsTable, table_name, out_table, &exception);

	return exception;
}

web_framework_exception_t wf_get_database_name(database_t implementation, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getDatabaseName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseName, &exception);

	return exception;
}

web_framework_exception_t wf_get_database_file_name(database_t implementation, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getDatabaseFileName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseFileName, &exception);

	return exception;
}
