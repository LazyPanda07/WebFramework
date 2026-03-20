#include "Managers/TaskBrokersManager.h"

#include <functional>

#include "TaskBroker/InternalTaskBroker.h"
#include "TaskBroker/RabbitMqTaskBroker.h"
#include "Utility/Utils.h"

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
			utility::logAndThrowException<logging::message::cantFindTaskBrokerImplementation, logging::category::taskBroker>(taskBrokerName);
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

		utility::logAndThrowException<logging::message::cantFindTaskBroker, logging::category::taskBroker>(name);
	}

	const TaskBroker& TaskBrokersManager::getTaskBroker(std::string_view name) const
	{
		for (const auto& [key, value] : taskBrokers)
		{
			if (value->getName() == name)
			{
				return *value;
			}
		}

		utility::logAndThrowException<logging::message::cantFindTaskBroker, logging::category::taskBroker>(name);
	}
}
