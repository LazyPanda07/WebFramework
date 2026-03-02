#pragma once

#include <TaskBroker/TaskExecutor.hpp>

#include <pybind11/pybind11.h>

namespace framework::task_broker
{
	class IPyTaskExecutor
	{
	public:
		IPyTaskExecutor() = default;

		virtual void operator ()(pybind11::dict data) = 0;

		virtual ~IPyTaskExecutor() = default;
	};

	class PyTaskExecutor : public IPyTaskExecutor
	{
	public:
		PyTaskExecutor() = default;

		void operator ()(pybind11::dict data) override;

		~PyTaskExecutor() = default;
	};

	class PyTaskExecutorWrapper : public TaskExecutor
	{
	private:
		IPyTaskExecutor& taskExecutor;

	public:
		PyTaskExecutorWrapper(IPyTaskExecutor& taskExecutor);

		void operator ()(const JsonObject& data) override;

		~PyTaskExecutorWrapper() = default;
	};
}
