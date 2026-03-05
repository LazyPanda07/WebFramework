#include "TaskBroker/TaskExecutors/CCTaskExecutor.h"

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw std::runtime_error(std::format("Can't load {} function", #name))

namespace framework::task_broker
{
	CCTaskExecutor::CCTaskExecutor(HMODULE module, void* implementation, std::string_view taskExecutorName) :
		implementation(implementation),
		executeFunction(utility::load<TaskExecuteSignature>(module, std::format("webFrameworkCCTaskExecute{}", taskExecutorName))),
		deleteFunction(utility::load<DeleteSignature>(module, std::format("webFrameworkCCDeleteTask{}", taskExecutorName)))
	{

	}

	void CCTaskExecutor::operator ()(json::JsonObject& data)
	{
		executeFunction(implementation, &data);
	}

	CCTaskExecutor::~CCTaskExecutor()
	{
		deleteFunction(implementation);
	}
}
