#include "TaskBroker/TaskExecutors/CSharpTaskExecutor.h"

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework::task_broker
{
	CSharpTaskExecutor::CSharpTaskExecutor(void* implementation) :
		implementation(implementation)
	{

	}

	void CSharpTaskExecutor::operator ()(json::JsonObject& data)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		runtime.getCallTaskExecutor()(implementation, &data);
	}

	CSharpTaskExecutor::~CSharpTaskExecutor()
	{
		if (implementation)
		{
			runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().free(implementation);

			implementation = nullptr;
		}
	}
}

#endif
