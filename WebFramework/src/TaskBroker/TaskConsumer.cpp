#include "TaskBroker/TaskConsumer.h"

#include <Log.h>

#include "Managers/TaskBrokersManager.h"
#include "Managers/TaskExecutorsManager.h"

namespace framework::task_broker
{
	void TaskConsumer::processTasks(TaskBroker& broker)
	{
		TaskExecutorsManager& taskExecutorsManager = TaskExecutorsManager::get();

		while (std::optional<json::JsonObject> task = broker.requestTask())
		{
			json::JsonObject& data = *task;
			const std::string& api = data["api"].get<std::string>();
			const std::string& taskExecutorName = data["name"].get<std::string>();

			try
			{
				TaskExecutor& taskExecutor = taskExecutorsManager.getTaskExecutor(api, taskExecutorName);

				taskRunner.addTask
				(
					[this, &taskExecutor, data = std::move(data)]() mutable
					{
						taskExecutor(data);
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
		if (Log::isValid())
		{
			Log::info("Start task consuming", "LogTaskBroker");
		}

		while (stillConsuming)
		{
			for (TaskBroker* broker : brokers)
			{
				this->processTasks(*broker);
			}

			std::this_thread::sleep_for(checkPeriod);
		}
	}

	TaskConsumer::TaskConsumer(const std::vector<std::string>& taskBrokerNames, size_t threadsNumber, std::chrono::milliseconds checkPeriod) :
		checkPeriod(checkPeriod),
		taskRunner(threadsNumber),
		stillConsuming(false)
	{
		for (const std::string& brokerName : taskBrokerNames)
		{
			brokers.emplace_back(&TaskBrokersManager::get().getTaskBroker(brokerName));

			if (Log::isValid())
			{
				Log::info("Add task broker: {}", "LogTaskBroker", brokerName);
			}
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
