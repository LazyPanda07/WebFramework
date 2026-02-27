#include "Managers/TaskBrokersManager.h"

namespace framework::task_broker
{
	TaskBrokersManager& TaskBrokersManager::get()
	{
		static TaskBrokersManager instance;

		return instance;
	}

	TaskBroker& TaskBrokersManager::getTaskBroker(std::string_view name)
	{
		for (auto& [key, value] : taskBrokers)
		{
			if (value->getName() == name)
			{
				return *value;
			}
		}

		throw std::runtime_error(std::format("Can't find {} task broker", name));
	}

	const TaskBroker& TaskBrokersManager::getTaskBroker(std::string_view name) const
	{
		for (auto& [key, value] : taskBrokers)
		{
			if (value->getName() == name)
			{
				return *value;
			}
		}

		throw std::runtime_error(std::format("Can't find {} task broker", name));
	}
}
