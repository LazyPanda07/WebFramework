#pragma once

#include <stdlib.h>

#include "../dll_handler.h"
#include "../json_object.h"

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
WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCTaskExecute##structName(void* implementation, void* jsonData)

#define DEFINE_DEFAULT_TASK_EXECUTOR(structName) typedef struct { char _; } structName; DEFINE_TASK_EXECUTOR(structName)

#define WF_GET_TASK_EXECUTOR_INSTANCE(structName) ((*structName) implementation)
#define WF_GET_TASK_EXECUTOR_ARGUMENTS() { .implementation = jsonData, .weak = true };
