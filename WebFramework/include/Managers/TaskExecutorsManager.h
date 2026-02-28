#pragma once

#include <vector>
#include <memory>

#include <Strings.h>

#include "TaskBroker/TaskExecutor.h"

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

	public:
		TaskExecutorsManager();

		TaskExecutor& getTaskExecutor(const std::string& apiName, const std::string& taskExeutorName) const;

		~TaskExecutorsManager() = default;
	};
}
