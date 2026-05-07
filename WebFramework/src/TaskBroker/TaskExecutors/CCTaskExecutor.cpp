#include "TaskBroker/TaskExecutors/CCTaskExecutor.h"

namespace framework::task_broker
{
	CCTaskExecutor::CCTaskExecutor(HMODULE module, void* implementation, std::string_view taskExecutorName) :
		implementation(implementation),
		executeFunction(utility::load<TaskExecuteSignature>(module, std::format("webFrameworkCCTaskExecute{}", taskExecutorName), true)),
		deleteFunction(utility::load<DeleteSignature>(module, std::format("webFrameworkCCDeleteTask{}", taskExecutorName), true))
	{

	}

	void CCTaskExecutor::execute(json::JsonObject& data, TaskExecutorContext& context)
	{
		executeFunction(implementation, &data, &context);
	}

	CCTaskExecutor::~CCTaskExecutor()
	{
		deleteFunction(implementation);
	}
}
