#pragma once

#include <TaskBroker/TaskExecutor.hpp>

#include <pybind11/pybind11.h>

namespace framework::task_broker
{
	class IPyTaskExecutor
	{
	public:
		IPyTaskExecutor() = default;

		virtual void execute(pybind11::dict data, const TaskExecutor::TaskExecutorContext& context) = 0;

		virtual ~IPyTaskExecutor() = default;
	};

	class PyTaskExecutor : public IPyTaskExecutor
	{
	public:
		PyTaskExecutor() = default;

		void execute(pybind11::dict data, const TaskExecutor::TaskExecutorContext& context) override;

		virtual ~PyTaskExecutor() = default;
	};

	class PyTaskExecutorWrapper : public TaskExecutor
	{
	private:
		IPyTaskExecutor& taskExecutor;

	public:
		PyTaskExecutorWrapper(IPyTaskExecutor& taskExecutor);

		void execute(const JsonObject& data, const TaskExecutorContext& context) override;

		~PyTaskExecutorWrapper() = default;
	};
}
