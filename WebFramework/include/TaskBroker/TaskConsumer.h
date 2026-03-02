#pragma once

#include <chrono>
#include <future>

#include <ThreadPool.h>

#include "TaskBroker.h"

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

	public:
		TaskConsumer(const std::vector<std::string>& taskBrokerNames, size_t threadsNumber, std::chrono::milliseconds checkPeriod);

		void run();

		~TaskConsumer();
	};
}
