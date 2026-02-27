#include "TaskBroker/InternalTaskBroker.h"

namespace framework::task_broker
{
	void InternalTaskBroker::enqueueTask(const json::JsonObject& data)
	{

	}

	constexpr std::string_view InternalTaskBroker::getName() const
	{
		return InternalTaskBroker::taskBrokerName;
	}
}
