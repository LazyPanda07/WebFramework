#include "TaskBroker/TaskExecutors/CXXTaskExecutor.h"

namespace framework::task_broker
{
	CXXTaskExecutor::CXXTaskExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		executeFunction(utility::load<TaskExecuteSignature>(module, "webFrameworkCXXTaskExecute", true)),
		deleteFunction(utility::load<DeleteSignature>(module, "webFrameworkCXXDeleteTask", true))
	{
		
	}

	void CXXTaskExecutor::operator ()(json::JsonObject& data)
	{
		executeFunction(implementation, &data);
	}

	CXXTaskExecutor::~CXXTaskExecutor()
	{
		deleteFunction(implementation);
	}
}
