#pragma once

#include <stdlib.h>

#include "../HttpRequest.h"
#include "../HttpResponse.h"
#include "../DLLHandler.h"
#include "../JsonParser.h"

typedef void* Executor;
typedef void* ExecutorSettings;

#ifdef __LINUX__
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API __attribute__((visibility("default"))) __attribute__((used))
#else
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API __declspec(dllexport)
#endif

/**
 * @brief Behavior of created Executor
 */
typedef enum ExecutorType
{
	/**
	 * @brief Create Executor for each client
	 */
	STATEFUL_EXECUTOR,
	/**
	 * @brief Create Executor once while server is initializing
	 */
	STATELESS_EXECUTOR,
	/**
	 * @brief Create Executor for each client. If server type is threadPool then DO methods runs in thread pool
	 */
	HEAVY_OPERATION_STATEFUL_EXECUTOR,
	/**
	 * @brief Create Executor once while server is initializing. If server type is threadPool then DO methods runs in thread pool
	 */
	HEAVY_OPERATION_STATELESS_EXECUTOR
} ExecutorType_t;

/**
 * @brief When Executors created
 */
typedef enum LoadType
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
} LoadType_t;

/**
* Create custom Executor function
* Used for loading function that creates Executor instance
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
* Create custom Executor function
* Used for loading function that creates BaseExecutor subclass
* @param structName Create empty struct for stateless executors
*/
#define DEFINE_DEFAULT_EXECUTOR(structName, executorType) typedef struct { char _; } structName; DEFINE_EXECUTOR(structName, executorType)

/**
 * @brief Available Executor methods
 */
typedef enum Methods
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
} Methods_t;

/**
 * Create method function
 * @param structName Executor name
 * @param method One of Methods value
 * @param requestVariableName Input HTTPRequest variable name
 * @param responseVariableName Input HTTPResponse variable name
 */
#define DEFINE_EXECUTOR_METHOD(structName, method, requestVariableName, responseVariableName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDo##method##structName(Executor executor, HTTPRequest requestVariableName, HTTPResponse responseVariableName)

/**
 * Create initialization function
 * @param structName Executor name
 */
#define DEFINE_EXECUTOR_INIT(structName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCExecutorInit##structName(Executor executor, ExecutorSettings settings)

/**
 * Create destroy function that would be called if Executor is destroyed
 * @param structName Executor name
 */
#define DEFINE_EXECUTOR_DESTROY(structName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDestroyExecutor##structName(Executor executor)

/**
 * @brief Get JSON structed values from initParameters section from settings file
 * @param implementation ExecutorSettings instance
 * @param result JSON structed values
 * @return Error if occurred
 */
WebFrameworkException getExecutorInitParameters(ExecutorSettings implementation, JSONParser* result);

/**
 * @brief Get executor name
 * @param implementation ExecutorSettings instance
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getExecutorName(ExecutorSettings implementation, WebFrameworkString* result);

/**
 * @brief Get filter by User-Agent header
 * @param implementation ExecutorSettings instance
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getExecutorUserAgentFilter(ExecutorSettings implementation, WebFrameworkString* result);

/**
 * @brief Get API language of Executor
 * @param implementation ExecutorSettings instance
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getExecutorAPIType(ExecutorSettings implementation, WebFrameworkString* result);

/**
 * @brief Get LoadType of Executor
 * @param implementation ExecutorSettings instance
 * @param result String with executor name. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getExecutorLoadType(ExecutorSettings implementation, LoadType_t* result);
