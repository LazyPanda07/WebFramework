#pragma once

#include <unordered_map>
#include <format>

#include "TaskBroker/TaskBroker.h"

namespace framework::task_broker
{
	class TaskBrokersManager
	{
	private:
		std::unordered_map<size_t, std::unique_ptr<TaskBroker>> taskBrokers;

	private:
		TaskBrokersManager() = default;

		TaskBrokersManager(const TaskBrokersManager&) = delete;

		TaskBrokersManager(TaskBrokersManager&&) noexcept = delete;

		TaskBrokersManager& operator =(const TaskBrokersManager&) = delete;

		TaskBrokersManager& operator =(TaskBrokersManager&&) noexcept = delete;

		~TaskBrokersManager() = default;

	public:
		static TaskBrokersManager& get();

		void addTaskBroker(std::string_view taskBrokerName);

		template<std::derived_from<TaskBroker> T, typename... Args>
		void addTaskBroker(Args&&... args);

		TaskBroker& getTaskBroker(std::string_view name);

		const TaskBroker& getTaskBroker(std::string_view name) const;

		template<std::derived_from<TaskBroker> T>
		T& getTaskBroker();

		template<std::derived_from<TaskBroker> T>
		const T& getTaskBroker() const;
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

		throw std::runtime_error(std::format("Can't find {} task broker", T::taskBrokerName));
	}

	template<std::derived_from<TaskBroker> T>
	const T& TaskBrokersManager::getTaskBroker() const
	{
		size_t key = typeid(T).hash_code();

		if (auto it = taskBrokers.find(key); it != taskBrokers.end())
		{
			return *static_cast<T*>(it->second);
		}

		throw std::runtime_error(std::format("Can't find {} task broker", T::taskBrokerName));
	}
}
