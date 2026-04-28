#include "Managers/TaskExecutorsManager.h"

#include "Managers/RuntimesManager.h"
#include "Utility/Sources.h"
#include "Utility/Utils.h"

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

	void TaskExecutorsManager::initTaskExecutor(const std::vector<utility::TaskExecutorsSettings>& taskExecutorsSettings)
	{
		runtime::RuntimesManager& manager = runtime::RuntimesManager::get();

		for (const auto& [sourcePath, classNames, api] : taskExecutorsSettings)
		{
			ValueType& value = taskExecutors[api];
			std::vector<std::pair<utility::LoadSource, std::string>> loadSources = utility::loadSources({ sourcePath.string() });

			if (loadSources.empty())
			{
				utility::logAndThrowException<logging::message::cantLoadTaskExecutor, logging::category::taskExecutor>(sourcePath.string());
			}

			if (Log::isValid())
			{
				Log::info<logging::message::foundTaskExecutorSource, logging::category::taskExecutor>(loadSources.front().second);
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

	void TaskExecutorsManager::createTaskConsumer(const std::vector<std::string>& taskBrokerNames, size_t threadsNumber, std::chrono::milliseconds checkPeriod, TaskBrokersManager& taskBrokersManager, WebFramework& frameworkInstance)
	{
		consumer = std::make_unique<TaskConsumer>(taskBrokerNames, threadsNumber, checkPeriod, *this, taskBrokersManager, frameworkInstance);
	}

	void TaskExecutorsManager::runTaskConsumer(std::shared_ptr<ResourceExecutor> resources)
	{
		if (consumer)
		{
			consumer->run(resources);
		}
	}

	TaskExecutor& TaskExecutorsManager::getTaskExecutor(const std::string& apiName, const std::string& taskExecutorName) const
	{
		const ValueType& executors = taskExecutors.at(apiName);

		if (auto it = executors.find(taskExecutorName); it != executors.end())
		{
			return *it->second;
		}

		utility::logAndThrowException<logging::message::cantFindApiTaskExecutor, logging::category::taskExecutor>(taskExecutorName, apiName);
	}
}
