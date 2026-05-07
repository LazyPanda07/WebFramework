#include "TaskBroker/TaskExecutors/CXXTaskExecutor.h"

namespace framework::task_broker
{
	CXXTaskExecutor::CXXTaskExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		executeFunction(utility::load<TaskExecuteSignature>(module, "webFrameworkCXXTaskExecute", true)),
		deleteFunction(utility::load<DeleteSignature>(module, "webFrameworkCXXDeleteTask", true))
	{
		
	}

	void CXXTaskExecutor::execute(json::JsonObject& data, TaskExecutorContext& context)
	{
		executeFunction(implementation, &data, &context);
	}

	CXXTaskExecutor::~CXXTaskExecutor()
	{
		deleteFunction(implementation);
	}
}
