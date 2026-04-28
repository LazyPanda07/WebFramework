#pragma once

#include <stdlib.h>

#include "dll_handler.h"
#include "json_object.h"
#include "databases/database.h"
#include "databases/implementations/default_database.h"
#include "databases/implementations/redis_database.h"
#include "databases/implementations/sqlite_database.h"

typedef void* task_executor_context_t;

/**
* Macro for each TaskExecutor subclass
* Used for loading function that creates TaskExecutor subclass
*/
#define DEFINE_TASK_EXECUTOR(structName) WEB_FRAMEWORK_FUNCTIONS_API void* create##structName##TaskCCInstance()	\
{	\
	return malloc(sizeof(structName));	\
}	\
	\
WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCDeleteTask##structName(void* implementation)	\
{	\
	free((structName*)implementation);	\
}	\
	\
WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCTaskExecute##structName(void* implementation, void* jsonData, void* context)

#define DEFINE_DEFAULT_TASK_EXECUTOR(structName) typedef struct { char _; } structName; DEFINE_TASK_EXECUTOR(structName)

#define WF_GET_TASK_EXECUTOR_INSTANCE(structName) ((*structName)implementation)
#define WF_GET_TASK_EXECUTOR_ARGUMENTS() { .implementation = jsonData, .weak = true }
#define WF_GET_TASK_EXECUTOR_CONTEXT() ((task_executor_context_t)context)

/**
 * @brief Get file content
 * @param implementation task_executor_context_t instance
 * @param filePath Path to asset file from assets folder
 * @param result File content. Delete with free function
 * @param size File content size
 * @return Error if occurred
 */
web_framework_exception_t wf_get_file_task_executor_context(task_executor_context_t implementation, const char* file_path, char** result, size_t* size);

/**
 * @brief Process static files like .md
 * @param implementation task_executor_context_t instance
 * @param fileData Static file content
 * @param size fileData size
 * @param fileExtension Supported processing extension
 * @param result Processed data
 * @param resultSize Processed data size
 * @return Error if occurred
 */
web_framework_exception_t wf_process_static_file_task_executor_context(task_executor_context_t implementation, const char* file_data, size_t size, const char* file_extension, char** result, size_t* result_size);

/**
 * @brief Process .wfdp files
 * @param implementation task_executor_context_t instance
 * @param fileData WFDP file content
 * @param arguments Arguments for processing .wfdp file
 * @param result Processed data
 * @param resultSize Processed data size
 * @return Error if occurred
 */
web_framework_exception_t wf_process_dynamic_file_task_executor_context(task_executor_context_t implementation, const char* file_data, size_t size, const json_object_t* arguments, char** result, size_t* result_size);

/**
 * @brief Tries to get database or create if can't get
 * @param implementation task_executor_context_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param result database_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_or_create_database_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, database_t* result);

/**
 * @brief Get database
 * @param implementation task_executor_context_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param result database_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_database_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, database_t* result);

/**
 * @brief Tries to get table from database or create if can't get
 * @param implementation task_executor_context_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param tableName Name of table
 * @param createTableQuery Create table query if can't get table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_or_create_table_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, const char* table_name, const char* create_table_query, table_t* result);

/**
 * @brief Get table from database
 * @param implementation task_executor_context_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param tableName Name of table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_table_task_executor_context(task_executor_context_t implementation, const char* database_name, const char* implementationName, const char* table_name, table_t* result);
