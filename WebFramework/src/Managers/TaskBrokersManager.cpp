#include "Managers/TaskBrokersManager.h"

#include <functional>

#include "TaskBroker/InternalTaskBroker.h"
#include "TaskBroker/RabbitMqTaskBroker.h"

namespace framework::task_broker
{
	TaskBrokersManager& TaskBrokersManager::get()
	{
		static TaskBrokersManager instance;

		return instance;
	}

	void TaskBrokersManager::addTaskBroker(std::string_view taskBrokerName, const json::JsonObject& settings)
	{
		static const std::unordered_map<std::string_view, std::function<void()>> creators =
		{
			{ InternalTaskBroker::taskBrokerName, [this, &settings]() { return this->addTaskBroker<InternalTaskBroker>(settings); }},
			{ RabbitMqTaskBroker::taskBrokerName, [this, &settings]() { return this->addTaskBroker<RabbitMqTaskBroker>(settings); }}
		};

		if (auto it = creators.find(taskBrokerName); it != creators.end())
		{
			it->second();
		}
		else
		{
			throw std::runtime_error(std::format("Can't find task broker implementation with {} name", taskBrokerName));
		}
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
