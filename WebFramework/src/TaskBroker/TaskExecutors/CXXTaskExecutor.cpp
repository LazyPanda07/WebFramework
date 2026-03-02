#include "TaskBroker/TaskExecutors/CXXTaskExecutor.h"

#include <format>

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw std::runtime_error(std::format("Can't load {} function", #name))

namespace framework::task_broker
{
	CXXTaskExecutor::CXXTaskExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		executeFunction(utility::load<TaskExecuteSignature>(module, "webFrameworkCXXTaskExecute")),
		deleteFunction(utility::load<DeleteSignature>(module, "webFrameworkCXXDeleteTask"))
	{
		ASSERT_LOAD_FUNCTION(executeFunction);
		ASSERT_LOAD_FUNCTION(deleteFunction);
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
