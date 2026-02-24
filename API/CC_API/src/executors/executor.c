#include "executors/executor.h"

web_framework_exception_t wf_register_dynamic_function_executor_settings(executor_settings_t implementation, const char* function_name, const char* (*function)(const char** arguments, size_t arguments_number), void(*deleter)(char* result))
{
	web_framework_exception_t exception = NULL;

	typedef void (*registerDynamicFunctionExecutorSettings)(void* implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result), void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(registerDynamicFunctionExecutorSettings, function_name, function, deleter, &exception);

	return exception;
}

web_framework_exception_t wf_get_executor_init_parameters(executor_settings_t implementation, json_parser_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorInitParameters)(void* implementation, void** exception);

	web_framework_string_t temp = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorInitParameters, &exception);

	if (exception)
	{
		return exception;
	}

	exception = wf_create_json_parser_from_string(wf_get_data_from_string(temp), result);

	wf_delete_string(temp);

	return exception;
}

web_framework_exception_t wf_get_executor_name(executor_settings_t implementation, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorName, &exception);

	return exception;
}

web_framework_exception_t wf_get_executor_user_agent_filter(executor_settings_t implementation, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorUserAgentFilter)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorUserAgentFilter, &exception);

	return exception;
}

web_framework_exception_t wf_get_executor_api_type(executor_settings_t implementation, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorAPIType)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorAPIType, &exception);

	return exception;
}

web_framework_exception_t wf_get_executor_load_type(executor_settings_t implementation, load_type_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int (*getExecutorLoadType)(void* implementation, void** exception);

	*result = (load_type_t)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorLoadType, &exception);

	return exception;
}

web_framework_exception_t wf_get_or_create_database_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, database_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getOrCreateDatabaseExecutorSettings)(void* implementation, const char* databaseName, const char* implementationName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateDatabaseExecutorSettings, database_name, implementationName, &exception);

	return exception;
}

web_framework_exception_t wf_get_database_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, database_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getDatabaseExecutorSettings)(void* implementation, const char* databaseName, const char* implementationName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseExecutorSettings, database_name, implementationName, &exception);

	return exception;
}

web_framework_exception_t wf_get_or_create_table_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, const char* table_name, const char* create_table_query, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getOrCreateTableExecutorSettings)(void* implementation, const char* databaseName, const char* implementationName, const char* tableName, const char* createTableQuery, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateTableExecutorSettings, database_name, implementationName, table_name, create_table_query, &exception);

	return exception;
}

web_framework_exception_t wf_get_table_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, const char* table_name, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getTableExecutorSettings)(void* implementation, const char* databaseName, const char* implementationName, const char* tableName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTableExecutorSettings, database_name, implementationName, table_name, &exception);

	return exception;
}
