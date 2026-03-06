#include "Managers/TaskExecutorsManager.h"

#include "Managers/RuntimesManager.h"
#include "Utility/Sources.h"

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
		runtime::RuntimesManager& manager = runtime::RuntimesManager::get();

		for (const auto& [sourcePath, classNames, api] : taskExecutorsSettings)
		{
			ValueType& value = taskExecutors[api];
			std::vector<std::pair<utility::LoadSource, std::string>> loadSources = utility::loadSources({ sourcePath.string() });

			if (loadSources.empty())
			{
				if (Log::isValid())
				{
					Log::error("Can't load task executors from {}", "LogTaskExecutor", sourcePath.string());
				}

				continue;
			}

			if (Log::isValid())
			{
				Log::info("Found load source with path: {}", "LogTaskExecutor", loadSources.front().second);
			}

			const auto& [loadSource, actualSourcePath] = loadSources.front();
			runtime::Runtime& runtime = manager.getRuntime(utility::getExecutorApiType(api));

			runtime.initializeWebFramework(loadSource);

			for (const std::string& name : classNames)
			{
				value.try_emplace(name, runtime.createTaskExecutor(name, loadSource));
			}
		}
	}

	void TaskExecutorsManager::createTaskConsumer(const std::vector<std::string>& taskBrokerNames, size_t threadsNumber, std::chrono::milliseconds checkPeriod)
	{
		consumer = std::make_unique<TaskConsumer>(taskBrokerNames, threadsNumber, checkPeriod);
	}

	void TaskExecutorsManager::runTaskConsumer()
	{
		if (consumer)
		{
			consumer->run();
		}
	}

	TaskExecutor& TaskExecutorsManager::getTaskExecutor(const std::string& apiName, const std::string& taskExecutorName) const
	{
		const ValueType& executors = taskExecutors.at(apiName);

		if (auto it = executors.find(taskExecutorName); it != executors.end())
		{
			return *it->second;
		}

		throw std::runtime_error(std::format("Can't find TaskExecutor with name: {} from {} API", taskExecutorName, apiName));
	}
}
