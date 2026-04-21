#include "TaskBroker/TaskConsumer.h"

#include "Utility/Utils.h"
#include "Managers/TaskExecutorsManager.h"

namespace framework::task_broker
{
	void TaskConsumer::processTasks(TaskBroker& broker)
	{
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
						taskExecutor(data["arguments"]);
					}
				);
			}
			catch (const std::exception& e)
			{
				if (Log::isValid())
				{
					Log::error<logging::message::cantProcessTask, logging::category::taskExecutor>(e.what(), broker.getName());
				}
			}
		}
	}

	void TaskConsumer::consume()
	{
		if (Log::isValid())
		{
			Log::info<logging::message::startTaskConsuming, logging::category::taskBroker>();
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

	TaskConsumer::TaskConsumer(const std::vector<std::string>& taskBrokerNames, size_t threadsNumber, std::chrono::milliseconds checkPeriod, TaskExecutorsManager& taskExecutorsManager, TaskBrokersManager& taskBrokerManager) :
		checkPeriod(checkPeriod),
		taskRunner(threadsNumber),
		stillConsuming(false),
		taskExecutorsManager(taskExecutorsManager)
	{
		for (const std::string& brokerName : taskBrokerNames)
		{
			brokers.emplace_back(&taskBrokerManager.getTaskBroker(brokerName));

			if (Log::isValid())
			{
				Log::info<logging::message::addTaskBroker, logging::category::taskBroker>(brokerName);
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
