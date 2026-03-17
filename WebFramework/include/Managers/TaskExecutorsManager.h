#pragma once

#include <vector>
#include <memory>

#include <Strings.h>

#include "TaskBroker/TaskExecutors/TaskExecutor.h"
#include "Utility/TaskExecutorsSettings.h"
#include "TaskBroker/TaskConsumer.h"

namespace framework::task_broker
{
	class TaskExecutorsManager
	{
	private:
		template<typename T>
		using StringMapType = ::utility::strings::string_based_unordered_map<T>;

		using ValueType = StringMapType<std::unique_ptr<TaskExecutor>>;
		using MapType = StringMapType<ValueType>;

	private:
		MapType taskExecutors;
		std::unique_ptr<TaskConsumer> consumer;

	private:
		TaskExecutorsManager();

		TaskExecutorsManager(const TaskExecutorsManager&) = delete;

		TaskExecutorsManager(TaskExecutorsManager&&) noexcept = delete;

		TaskExecutorsManager& operator =(const TaskExecutorsManager&) = delete;

		TaskExecutorsManager& operator =(TaskExecutorsManager&&) noexcept = delete;

		~TaskExecutorsManager() = default;

	public:
		static TaskExecutorsManager& get();

		void initTaskExecutor(const std::vector<utility::TaskExecutorsSettings>& taskExecutorsSettings);

		void createTaskConsumer(const std::vector<std::string>& taskBrokerNames, size_t threadsNumber, std::chrono::milliseconds checkPeriod);

		void runTaskConsumer();

		TaskExecutor& getTaskExecutor(const std::string& apiName, const std::string& taskExecutorName) const;		
	};
}
