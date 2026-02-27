#include "Managers/MessageBrokersManager.h"

namespace framework::message_broker
{
	MessageBrokersManager& MessageBrokersManager::get()
	{
		static MessageBrokersManager instance;

		return instance;
	}

	MessageBroker& MessageBrokersManager::getMessageBroker(std::string_view name)
	{
		for (auto& [key, value] : messageBrokers)
		{
			if (value->getName() == name)
			{
				return *value;
			}
		}

		throw std::runtime_error(std::format("Can't find {} message broker", name));
	}

	const MessageBroker& MessageBrokersManager::getMessageBroker(std::string_view name) const
	{
		for (auto& [key, value] : messageBrokers)
		{
			if (value->getName() == name)
			{
				return *value;
			}
		}

		throw std::runtime_error(std::format("Can't find {} message broker", name));
	}
}
