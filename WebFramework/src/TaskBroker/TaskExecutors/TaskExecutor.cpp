#include "TaskBroker/TaskExecutors/TaskExecutor.h"

namespace framework::task_broker
{
	TaskExecutor::TaskExecutorContext::TaskExecutorContext(std::shared_ptr<ResourceExecutor> resources, WebFramework& frameworkInstance) :
		resources(resources),
		frameworkInstance(frameworkInstance)
	{

	}

	TaskExecutor::TaskExecutorContext::~TaskExecutorContext()
	{
		for (interfaces::IDatabase* database : databases)
		{
			delete database;
		}
	}
}
