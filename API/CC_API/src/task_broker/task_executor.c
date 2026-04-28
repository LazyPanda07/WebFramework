#include "task_broker/task_executor.h"

typedef struct file_buffer
{
	char** data;
	size_t* size;
} file_buffer_t;

static void __fill_file_buffer(const char* data, size_t size, void* buffer)
{
	file_buffer_t* fileBuffer = (file_buffer_t*)buffer;
	char** result = fileBuffer->data;
	size_t* resultSize = fileBuffer->size;

	*result = malloc(size + 1);

	if (!*result)
	{
		fprintf(stderr, "Can't allocate memory for file buffer\n");

		exit(7);
	}

	(*result)[size] = '\0';
	*resultSize = size;

	memcpy(*result, data, size);
}

web_framework_exception_t wf_get_file_task_executor_context(task_executor_context_t implementation, const char* file_path, char** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getFileTaskExecutorContext)(void* implementation, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFileTaskExecutorContext, file_path, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_process_static_file_task_executor_context(task_executor_context_t implementation, const char* file_data, size_t size, const char* file_extension, char** result, size_t* result_size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*processStaticFileTaskExecutorContext)(void* implementation, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = result_size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processStaticFileTaskExecutorContext, file_data, size, file_extension, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_process_dynamic_file_task_executor_context(task_executor_context_t implementation, const char* file_data, size_t size, const json_object_t* arguments, char** result, size_t* result_size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*processDynamicFileTaskExecutorContext)(void* implementation, const char* fileData, size_t size, const void* arguments, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = result_size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processDynamicFileTaskExecutorContext, file_data, size, arguments->implementation, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_get_or_create_database_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, database_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getOrCreateDatabaseTaskExecutorContext)(void* implementation, const char* databaseName, const char* implementationName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateDatabaseTaskExecutorContext, database_name, implementationName, &exception);

	return exception;
}

web_framework_exception_t wf_get_database_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, database_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getDatabaseTaskExecutorContext)(void* implementation, const char* databaseName, const char* implementationName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseTaskExecutorContext, database_name, implementationName, &exception);

	return exception;
}

web_framework_exception_t wf_get_or_create_table_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, const char* table_name, const char* create_table_query, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getOrCreateTableTaskExecutorContext)(void* implementation, const char* databaseName, const char* implementationName, const char* tableName, const char* createTableQuery, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateTableTaskExecutorContext, database_name, implementationName, table_name, create_table_query, &exception);

	return exception;
}

web_framework_exception_t wf_get_table_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, const char* table_name, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getTableTaskExecutorContext)(void* implementation, const char* databaseName, const char* implementationName, const char* tableName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTableTaskExecutorContext, database_name, implementationName, table_name, &exception);

	return exception;
}
