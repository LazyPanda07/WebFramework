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

typedef enum
{
	STATEFUL_EXECUTOR,
	STATELESS_EXECUTOR,
	HEAVY_OPERATION_STATEFUL_EXECUTOR,
	HEAVY_OPERATION_STATELESS_EXECUTOR
} ExecutorType;

/**
* Macro for each BaseExecutor subclass
* Used for loading function that creates BaseExecutor subclass
*/
#define DECLARE_EXECUTOR(subclassName, executorType) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void* create##subclassName##CCInstance()	\
{	\
	return malloc(sizeof(subclassName));	\
}	\
	\
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API int webFrameworkCCGetType##subclassName(void* executor)	\
{	\
	return executorType;	\
}	\
	\
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDeleteExecutor##subclassName(void* executor)	\
{	\
	free((subclassName*)executor);	\
}

typedef enum
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
} Methods;

#define DECLARE_EXECUTOR_METHOD(subclassName, method, requestVariableName, responseVariableName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDo##method##subclassName(Executor executor, HTTPRequest requestVariableName, HTTPResponse responseVariableName)

#define DECLARE_EXECUTOR_INIT(subclassName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCExecutorInit##subclassName(Executor executor, ExecutorSettings settings)

#define DECLARE_EXECUTOR_DESTROY(subclassName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void webFrameworkCCDeleteExecutor##subclassName(Executor executor)

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void initializeWebFrameworkCC(const char* webFrameworkSharedLibraryPath)
{
	initializeWebFramework(webFrameworkSharedLibraryPath);
}
