#pragma once

#include <stdlib.h>

#include "../HTTPRequest.h"
#include "../HTTPResponse.h"
#include "../DLLHandler.h"

typedef void* Executor;
typedef void* ExecutorSettings;

#ifdef __LINUX__
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API __attribute__((visibility("default"))) __attribute__((used))
#else
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API __declspec(dllexport)
#endif

typedef enum ExecutorType
{
	STATEFUL_EXECUTOR,
	STATELESS_EXECUTOR,
	HEAVY_OPERATION_STATEFUL_EXECUTOR,
	HEAVY_OPERATION_STATELESS_EXECUTOR
} ExecutorType_t;

/**
* Macro for each BaseExecutor subclass
* Used for loading function that creates BaseExecutor subclass
* @param structName Already defined struct name
*/
#define DECLARE_EXECUTOR(structName, executorType) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void* create##structName##CCInstance()	\
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
* Macro for each BaseExecutor subclass
* Used for loading function that creates BaseExecutor subclass
* @param structName Create empty struct for stateless executors
*/
#define DECLARE_DEFAULT_EXECUTOR(structName, executorType) typedef struct { char _; } structName; DECLARE_EXECUTOR(structName, executorType)

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

#define DECLARE_EXECUTOR_METHOD(structName, method, requestVariableName, responseVariableName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDo##method##structName(Executor executor, HTTPRequest requestVariableName, HTTPResponse responseVariableName)

#define DECLARE_EXECUTOR_INIT(structName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCExecutorInit##structName(Executor executor, ExecutorSettings settings)

#define DECLARE_EXECUTOR_DESTROY(structName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDestroyExecutor##structName(Executor executor)

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void initializeWebFrameworkCC(const char* webFrameworkSharedLibraryPath)
{
	initializeWebFramework(webFrameworkSharedLibraryPath);
}
