#pragma once

#include "../DLLHandler.hpp"
#include "../JsonObject.hpp"

namespace framework::task_broker
{
	class TaskExecutor
	{
	public:
		TaskExecutor() = default;

		virtual void operator ()(const JsonObject& data) = 0;

		virtual ~TaskExecutor() = default;
	};
}

/**
* Macro for each TaskExecutor subclass
* Used for loading function that creates TaskExecutor subclass
*/
#define DEFINE_TASK_EXECUTOR(subclassName) WEB_FRAMEWORK_FUNCTIONS_API void* create##subclassName##TaskCXXInstance()	\
{	\
	return new subclassName();	\
}

#pragma region ExportFunctions
WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXTaskExecute(void* implementation, void* jsonData)
{
	framework::JsonObject data(jsonData);

	(*static_cast<framework::task_broker::TaskExecutor*>(implementation))(data);
}

WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXDeleteTask(void* implementation)
{
	delete static_cast<framework::task_broker::TaskExecutor*>(implementation);
}
#pragma endregion
