#pragma once

#include <stdlib.h>

#include "http_request.h"
#include "http_response.h"
#include "dll_handler.h"
#include "json_parser.h"

typedef void* executor_t;
typedef void* executor_settings_t;

/**
 * @brief Behavior of created executor_t
 */
typedef enum executor_type
{
	/**
	 * @brief Create executor_t for each client
	 */
	STATEFUL_EXECUTOR,
	/**
	 * @brief Create executor_t once while server is initializing
	 */
	STATELESS_EXECUTOR,
	/**
	 * @brief Create executor_t for each client. If server type is threadPool then DO methods runs in thread pool
	 */
	HEAVY_OPERATION_STATEFUL_EXECUTOR,
	/**
	 * @brief Create executor_t once while server is initializing. If server type is threadPool then DO methods runs in thread pool
	 */
	HEAVY_OPERATION_STATELESS_EXECUTOR
} executor_type_t;

/**
 * @brief When Executors created
 */
typedef enum load_type
{
	/**
	 * @brief Create at initialization
	 */
	INITIALIZATION,
	/**
	 * @brief Create if client connected
	 */
	DYNAMIC,
	/**
	 * @brief Signal that error happened
	 */
	NONE
} load_type_t;

/**
* Create custom executor_t function
* Used for loading function that creates executor_t instance
* @param structName Already defined struct name
*/
#define DEFINE_EXECUTOR(structName, executorType) WEB_FRAMEWORK_FUNCTIONS_API void* create##structName##CCInstance()	\
{	\
	return malloc(sizeof(structName));	\
}	\
	\
WEB_FRAMEWORK_FUNCTIONS_API int webFrameworkCCGetType##structName(void* executor)	\
{	\
	return executorType;	\
}	\
	\
WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCDeleteExecutor##structName(void* executor)	\
{	\
	free((structName*)executor);	\
}

/**
* Create custom executor_t function
* Used for loading function that creates executor_t subclass
* @param structName Create empty struct for stateless executors
*/
#define DEFINE_DEFAULT_EXECUTOR(structName, executorType) typedef struct { char _; } structName; DEFINE_EXECUTOR(structName, executorType)

/**
 * @brief Available executor_t methods
 */
typedef enum methods
{
	POST_METHOD,
	GET_METHOD,
	HEAD_METHOD,
	PUT_METHOD,
	DELETE_METHOD,
	PATCH_METHOD,
	OPTIONS_METHOD,
	TRACE_METHOD,
	CONNECT_METHOD,
} methods_t;

/**
 * Create method function
 * @param structName executor_t name
 * @param method One of Methods value
 * @param request_variable_name Input http_request_t variable name
 * @param response_variable_name Input http_response_t variable name
 */
#define DEFINE_EXECUTOR_METHOD(structName, method, request_variable_name, response_variable_name) WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCDo##method##structName(executor_t executor, http_request_t request_variable_name, http_response_t response_variable_name)

/**
 * Create initialization function
 * @param structName executor_t name
 */
#define DEFINE_EXECUTOR_INIT(structName) WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCExecutorInit##structName(executor_t executor, executor_settings_t settings)

/**
 * Create destroy function that would be called if executor_t is destroyed
 * @param structName executor_t name
 */
#define DEFINE_EXECUTOR_DESTROY(structName) WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCDestroyExecutor##structName(executor_t executor)

 /**
  * @brief Register function for processing .wfdp files
  * @param implementation executor_settings_t instance
  * @param functionName Function name
  * @param function Callback called in processing .wfdp files
  * @param deleter Deleter for values from function
  * @return
  */
web_framework_exception_t wf_register_dynamic_function_executor_settings(executor_settings_t implementation, const char* function_name, char* (*function)(json_object_t arguments), void(*deleter)(char* result));

/**
 * @brief Unregister function for processing .wfdp files
 * @param implementation executor_settings_t instance
 * @param functionName Function name
 * @return Error if occurred
 */
web_framework_exception_t wf_unregister_dynamic_function_executor_settings(executor_settings_t implementation, const char* functionName);

/**
 * @brief Check is WFDP function registered
 * @param implementation executor_settings_t instance
 * @param functionName Function name
 * @param result Is function registered
 * @return Error if occurred
 */
web_framework_exception_t wf_is_dynamic_function_registered_executor_settings(executor_settings_t implementation, const char* functionName, bool* result);

/**
 * @brief Get file content
 * @param implementation executor_settings_t instance
 * @param filePath Path to asset file from assets folder
 * @param result File content. Delete with free function
 * @param size File content size
 * @return Error if occurred
 */
web_framework_exception_t wf_get_file_executor_settings(executor_settings_t implementation, const char* file_path, char** result, size_t* size);

/**
 * @brief Process static files like .md
 * @param implementation executor_settings_t instance
 * @param fileData Static file content
 * @param size fileData size
 * @param fileExtension Supported processing extension
 * @param result Processed data
 * @param resultSize Processed data size
 * @return Error if occurred
 */
web_framework_exception_t wf_process_static_file_executor_settings(executor_settings_t implementation, const char* file_data, size_t size, const char* file_extension, char** result, size_t* result_size);

/**
 * @brief Process .wfdp files
 * @param implementation executor_settings_t instance
 * @param fileData WFDP file content
 * @param arguments Arguments for processing .wfdp file
 * @param result Processed data
 * @param resultSize Processed data size
 * @return Error if occurred
 */
web_framework_exception_t wf_process_dynamic_file_executor_settings(executor_settings_t implementation, const char* file_data, size_t size, const json_object_t* arguments, char** result, size_t* result_size);

/**
 * @brief Get Json structed values from initParameters section from settings file
 * @param implementation executor_settings_t instance
 * @param result Json structed values
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_init_parameters(executor_settings_t implementation, json_parser_t* result);

/**
 * @brief Get executor name
 * @param implementation executor_settings_t instance
 * @param result String with executor name. Delete with wf_delete_string function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_name(executor_settings_t implementation, web_framework_string_t* result);

/**
 * @brief Get filter by User-Agent header
 * @param implementation executor_settings_t instance
 * @param result Array of strings with user agent filters
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_user_agent_filter(executor_settings_t implementation, char*** result, size_t* result_size);

/**
 * @brief Get API language of executor_t
 * @param implementation executor_settings_t instance
 * @param result String with executor name. Delete with wf_delete_string function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_api_type(executor_settings_t implementation, web_framework_string_t* result);

/**
 * @brief Get LoadType of executor_t
 * @param implementation executor_settings_t instance
 * @param result String with executor name. Delete with wf_delete_string function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_load_type(executor_settings_t implementation, load_type_t* result);

/**
 * @brief Tries to get database or create if can't get
 * @param implementation executor_settings_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param result database_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_or_create_database_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, database_t* result);

/**
 * @brief Get database
 * @param implementation executor_settings_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param result database_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_database_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, database_t* result);

/**
 * @brief Tries to get table from database or create if can't get
 * @param implementation executor_settings_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param tableName Name of table
 * @param createTableQuery Create table query if can't get table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_or_create_table_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, const char* table_name, const char* create_table_query, table_t* result);

/**
 * @brief Get table from database
 * @param implementation executor_settings_t instance
 * @param databaseName database_t name
 * @param databaseName Macro from databases/implementations
 * @param tableName Name of table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_table_executor_settings(executor_settings_t implementation, const char* database_name, const char* implementationName, const char* table_name, table_t* result);
