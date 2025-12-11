#pragma once

#include <stdlib.h>

#include "../http_request_t.h"
#include "../http_response.h"
#include "../dll_handler.h"
#include "../json_parser.h"

typedef void* executor_t;
typedef void* executor_settings_t;

#ifdef __LINUX__
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API __attribute__((visibility("default"))) __attribute__((used))
#else
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API __declspec(dllexport)
#endif

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
#define DEFINE_EXECUTOR(structName, executorType) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void* create##structName##CCInstance()	\
{	\
	return malloc(sizeof(structName));	\
}	\
	\
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API int webFrameworkCCGetType##structName(void* executor)	\
{	\
	return executorType;	\
}	\
	\
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDeleteExecutor##structName(void* executor)	\
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
 * @param requestVariableName Input http_request_t variable name
 * @param responseVariableName Input http_response_t variable name
 */
#define DEFINE_EXECUTOR_METHOD(structName, method, requestVariableName, responseVariableName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDo##method##structName(executor_t executor, http_request_t requestVariableName, http_response_t responseVariableName)

/**
 * Create initialization function
 * @param structName executor_t name
 */
#define DEFINE_EXECUTOR_INIT(structName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCExecutorInit##structName(executor_t executor, executor_settings_t settings)

/**
 * Create destroy function that would be called if executor_t is destroyed
 * @param structName executor_t name
 */
#define DEFINE_EXECUTOR_DESTROY(structName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDestroyExecutor##structName(executor_t executor)

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
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_name(executor_settings_t implementation, web_framework_string_t* result);

/**
 * @brief Get filter by User-Agent header
 * @param implementation executor_settings_t instance
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_user_agent_filter(executor_settings_t implementation, web_framework_string_t* result);

/**
 * @brief Get API language of executor_t
 * @param implementation executor_settings_t instance
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_api_type(executor_settings_t implementation, web_framework_string_t* result);

/**
 * @brief Get LoadType of executor_t
 * @param implementation executor_settings_t instance
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_executor_load_type(executor_settings_t implementation, load_type_t* result);
