#include "TaskBroker/InternalTaskBroker.h"

#include <Log.h>

namespace framework::task_broker
{
	void InternalTaskBroker::enqueueTask(json::JsonObject&& data)
	{
		if (Log::isValid())
		{
			Log::info("Add task to internal task broker", "LogTaskBroker");
		}

		tasks.push(std::move(data));
	}

	std::optional<json::JsonObject> InternalTaskBroker::requestTask()
	{
		if (Log::isValid())
		{
			Log::info("Request task from {}. Currently has {} tasks", "LogTaskBroker", this->getName(), tasks.size());
		}

		return tasks.pop();
	}
}
