#pragma once

#include <chrono>
#include <future>

#include <ThreadPool.h>

#include "TaskBroker.h"
#include "Managers/TaskExecutorsManager.h"

namespace framework::task_broker
{
	class TaskConsumer
	{
	private:
		void processTasks(TaskBroker& broker);

	private:
		void consume();

	private:
		std::vector<TaskBroker*> brokers;
		std::future<void> consumeThread;
		std::chrono::seconds checkPeriod;
		threading::ThreadPool& taskRunner;
		TaskExecutorsManager& taskExecutorsManager;
		bool stillConsuming;

	public:
		TaskConsumer(const std::vector<std::string>& taskBrokerNames, threading::ThreadPool& taskRunner, TaskExecutorsManager& taskExecutorsManager, std::chrono::seconds checkPeriod = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(2500)));

		void run();

		~TaskConsumer();
	};
}
