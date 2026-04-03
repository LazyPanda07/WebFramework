#include "executors/executor.h"

typedef struct file_buffer
{
	char** data;
	size_t* size;
} file_buffer_t;

typedef struct user_filter_buffer
{
	char*** result;
	size_t* result_size;
} user_filter_buffer_t;

static void __fill_file_buffer(const char* data, size_t size, void* buffer)
{
	file_buffer_t* fileBuffer = (file_buffer_t*)buffer;
	char** result = fileBuffer->data;
	size_t* resultSize = fileBuffer->size;

	*result = malloc(size + 1);

	if (!*result)
	{
		fprintf(stderr, "Can't allocate memory for file buffer\n");

		exit(6);
	}

	(*result)[size] = '\0';
	*resultSize = size;

	memcpy(*result, data, size);
}

static void __init_user_agent_filter(size_t size, void* buffer)
{
	user_filter_buffer_t* temp = (user_filter_buffer_t*)buffer;

	*(temp->result) = malloc(sizeof(char**) * size);
}

static void __fill_user_agent_filter(const char* value, size_t index, void* buffer)
{
	user_filter_buffer_t* temp = (user_filter_buffer_t*)buffer;
	char** ptr = &((*temp->result)[index]);

	*ptr = calloc(strlen(value) + 1, sizeof(char));

	if (!*ptr)
	{
		fprintf(stderr, "Can't allocate memory for copying value in user agent filter\n");

		exit(7);
	}

	memcpy(*ptr, value, strlen(value));
}

web_framework_exception_t wf_register_dynamic_function_executor_settings(executor_settings_t implementation, const char* function_name, const char* (*function)(const char** arguments, size_t arguments_number), void(*deleter)(char* result))
{
	web_framework_exception_t exception = NULL;
	struct { void* function; void* deleter; } data =
	{
		.function = function,
		.deleter = deleter
	};

	typedef void (*registerDynamicFunctionClassExecutorSettings)(void* implementation, const char* functionName, const char* apiType, void* functionClass, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(registerDynamicFunctionClassExecutorSettings, function_name, "cc", &data, &exception);

	return exception;
}

web_framework_exception_t wf_unregister_dynamic_function_executor_settings(executor_settings_t implementation, const char* functionName)
{
	web_framework_exception_t exception = NULL;

	typedef void (*unregisterDynamicFunctionExecutorSettings)(void* implementation, const char* functionName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(unregisterDynamicFunctionExecutorSettings, functionName, &exception);

	return exception;
}

web_framework_exception_t wf_is_dynamic_function_registered_executor_settings(executor_settings_t implementation, const char* functionName, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*isDynamicFunctionRegisteredExecutorSettings)(void* implementation, const char* functionName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isDynamicFunctionRegisteredExecutorSettings, functionName, &exception);

	return exception;
}

web_framework_exception_t wf_get_file_executor_settings(executor_settings_t implementation, const char* file_path, const char** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getFileExecutorSettings)(void* implementation, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFileExecutorSettings, file_path, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_process_static_file_executor_settings(executor_settings_t implementation, const char* file_data, size_t size, const char* file_extension, const char** result, size_t* result_size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*processStaticFileExecutorSettings)(void* implementation, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = result_size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processStaticFileExecutorSettings, file_data, size, file_extension, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_process_dynamic_file_executor_settings(executor_settings_t implementation, const char* file_data, size_t size, const dynamic_pages_variable_t* variables, size_t variables_size, const char** result, size_t* result_size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*processDynamicFileExecutorSettings)(void* implementation, const char* fileData, size_t size, const void* variables, size_t variablesSize, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = result_size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processDynamicFileExecutorSettings, file_data, size, variables, variables_size, __fill_file_buffer, &buffer, &exception);

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

web_framework_exception_t wf_get_executor_user_agent_filter(executor_settings_t implementation, char*** result, size_t* result_size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getExecutorUserAgentFilter)(void* implementation, void(*initUserAgentFilterBuffer)(size_t size, void* buffer), void(*addUserAgentFilter)(const char* value, size_t index, void* buffer), void* buffer, void** exception);

	user_filter_buffer_t temp = { .result = result, .result_size = result_size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorUserAgentFilter, __init_user_agent_filter, __fill_user_agent_filter, &temp, &exception);

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
