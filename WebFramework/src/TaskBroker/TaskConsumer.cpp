#include "TaskBroker/TaskConsumer.h"

#include <Log.h>

#include "Managers/TaskBrokersManager.h"

namespace framework::task_broker
{
	void TaskConsumer::processTasks(TaskBroker& broker)
	{
		while (std::optional<json::JsonObject> task = broker.requestTask())
		{
			json::JsonObject& data = *task;
			const std::string& api = data["api"].get<std::string>();
			const std::string& taskExecutorName = data["name"].get<std::string>();
			TaskExecutor& taskExecutor = taskExecutorsManager.getTaskExecutor(api, taskExecutorName);

			try
			{
				taskRunner.addTask
				(
					[this, &taskExecutor, data = std::move(data)]() mutable
					{
						taskExecutor.execute(std::move(data));
					}
				);
			}
			catch (const std::exception& e)
			{
				if (Log::isValid())
				{
					Log::error("Can't process task: {}. TaskBroker: {}", "LogTaskExecutor", e.what(), broker.getName());
				}
			}
		}
	}

	void TaskConsumer::consume()
	{
		while (stillConsuming)
		{
			for (TaskBroker* broker : brokers)
			{
				this->processTasks(*broker);
			}

			std::this_thread::sleep_for(checkPeriod);
		}
	}

	TaskConsumer::TaskConsumer(const std::vector<std::string>& taskBrokerNames, threading::ThreadPool& taskRunner, TaskExecutorsManager& taskExecutorsManager, std::chrono::seconds checkPeriod) :
		checkPeriod(checkPeriod),
		taskRunner(taskRunner),
		taskExecutorsManager(taskExecutorsManager),
		stillConsuming(false)
	{
		for (const std::string& brokerName : taskBrokerNames)
		{
			brokers.emplace_back(&TaskBrokersManager::get().getTaskBroker(brokerName));
		}
	}

	void TaskConsumer::run()
	{
		stillConsuming = true;

		consumeThread = std::async(std::launch::async, &TaskConsumer::consume, this);
	}

	TaskConsumer::~TaskConsumer()
	{
		stillConsuming = false;
	}
}
