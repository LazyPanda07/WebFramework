#include "TaskBroker/InternalTaskBroker.h"

namespace framework::task_broker
{
	void InternalTaskBroker::enqueueTask(json::JsonObject&& data)
	{
		tasks.push(std::move(data));
	}

	std::optional<json::JsonObject> InternalTaskBroker::requestTask()
	{
		return tasks.pop();
	}

	std::string_view InternalTaskBroker::getName() const
	{
		return InternalTaskBroker::taskBrokerName;
	}
}
