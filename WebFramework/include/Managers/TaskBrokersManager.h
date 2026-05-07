#pragma once

#include <unordered_map>

#include "TaskBroker/TaskBroker.h"
#include "Utility/Utils.h"

namespace framework::task_broker
{
	class TaskBrokersManager
	{
	private:
		std::unordered_map<size_t, std::unique_ptr<TaskBroker>> taskBrokers;

	public:
		TaskBrokersManager() = default;

		void addTaskBroker(std::string_view taskBrokerName, const json::JsonObject& settings);

		template<std::derived_from<TaskBroker> T, typename... Args>
		void addTaskBroker(Args&&... args);

		TaskBroker& getTaskBroker(std::string_view name);

		const TaskBroker& getTaskBroker(std::string_view name) const;

		template<std::derived_from<TaskBroker> T>
		T& getTaskBroker();

		template<std::derived_from<TaskBroker> T>
		const T& getTaskBroker() const;

		~TaskBrokersManager() = default;
	};
}

namespace framework::task_broker
{
	template<std::derived_from<TaskBroker> T, typename... Args>
	void TaskBrokersManager::addTaskBroker(Args&&... args)
	{
		size_t key = typeid(T).hash_code();

		if (!taskBrokers.contains(key))
		{
			taskBrokers.try_emplace(key, std::make_unique<T>(std::forward<Args>(args)...));
		}
	}

	template<std::derived_from<TaskBroker> T>
	T& TaskBrokersManager::getTaskBroker()
	{
		size_t key = typeid(T).hash_code();

		if (auto it = taskBrokers.find(key); it != taskBrokers.end())
		{
			return *static_cast<T*>(it->second);
		}

		utility::logAndThrowException<logging::message::cantFindTaskBroker, logging::category::taskBroker>(T::taskBrokerName);
	}

	template<std::derived_from<TaskBroker> T>
	const T& TaskBrokersManager::getTaskBroker() const
	{
		size_t key = typeid(T).hash_code();

		if (auto it = taskBrokers.find(key); it != taskBrokers.end())
		{
			return *static_cast<T*>(it->second);
		}

		utility::logAndThrowException<logging::message::cantFindTaskBroker, logging::category::taskBroker>(T::taskBrokerName);
	}
}
