#include "TaskBroker/InternalTaskBroker.h"

namespace framework::task_broker
{
	InternalTaskBroker::InternalTaskBroker(const json::JsonObject& settings)
	{

	}

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
