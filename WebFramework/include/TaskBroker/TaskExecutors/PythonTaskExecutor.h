#pragma once

#include "TaskExecutor.h"

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

namespace framework::task_broker
{
	class PythonTaskExecutor : public TaskExecutor
	{
	private:
		pybind11::object* implementation;

	public:
		PythonTaskExecutor(pybind11::object* implementation);

		void operator ()(json::JsonObject& data) override;

		~PythonTaskExecutor();
	};
}

#endif
