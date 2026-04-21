#pragma once

#include <chrono>
#include <future>

#include <ThreadPool.h>

#include "TaskBroker.h"
#include "Managers/TaskBrokersManager.h"

namespace framework::task_broker
{
	class TaskExecutorsManager;
}

namespace framework::task_broker
{
	class TaskConsumer
	{
	private:
		void processTasks(TaskBroker& broker);

		void consume();

	private:
		std::vector<TaskBroker*> brokers;
		std::future<void> consumeThread;
		std::chrono::milliseconds checkPeriod;
		threading::ThreadPool taskRunner;
		bool stillConsuming;
		TaskExecutorsManager& taskExecutorsManager;

	public:
		TaskConsumer(const std::vector<std::string>& taskBrokerNames, size_t threadsNumber, std::chrono::milliseconds checkPeriod, TaskExecutorsManager& taskExecutorsManager, TaskBrokersManager& taskBrokerManager);

		void run();

		~TaskConsumer();
	};
}
