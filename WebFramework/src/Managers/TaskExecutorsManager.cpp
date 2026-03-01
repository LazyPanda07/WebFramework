#include "Managers/TaskExecutorsManager.h"

#include "Managers/RuntimesManager.h"

namespace framework::task_broker
{
	TaskExecutorsManager::TaskExecutorsManager()
	{
		runtime::RuntimesManager& manager = runtime::RuntimesManager::get();

		for (const runtime::Runtime& runtime : manager)
		{
			taskExecutors.try_emplace(std::string(runtime.getName()));
		}
	}

	TaskExecutorsManager& TaskExecutorsManager::get()
	{
		static TaskExecutorsManager instance;

		return instance;
	}

	void TaskExecutorsManager::initTaskExecutor(const std::vector<utility::TaskExecutorsSettings>& taskExecutorsSettings)
	{
		for (const utility::TaskExecutorsSettings& taskExecutorSettings : taskExecutorsSettings)
		{
			
		}
	}

	TaskExecutor& TaskExecutorsManager::getTaskExecutor(const std::string& apiName, const std::string& taskExeutorName) const
	{
		const ValueType& executors = taskExecutors.at(apiName);

		if (auto it = executors.find(taskExeutorName); it != executors.end())
		{
			return *it->second;
		}

		throw std::runtime_error(std::format("Can't find TaskExecutor with name: {} from {} API", taskExeutorName, apiName));
	}
}
