#include "MessageBroker/InternalMessageBroker.h"

namespace framework::message_broker
{
	void InternalMessageBroker::enqueueTask(const json::JsonObject& data)
	{

	}

	constexpr std::string_view InternalMessageBroker::getName() const
	{
		return InternalMessageBroker::messageBrokerName;
	}
}
