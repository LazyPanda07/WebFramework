#include "TaskBroker/PyTaskExecutor.h"

#include <sstream>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace framework::task_broker
{
	void PyTaskExecutor::execute(py::dict data, const TaskExecutor::TaskExecutorContext& context)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			IPyTaskExecutor,
			execute,
			data,
			context
		);
	}

	PyTaskExecutorWrapper::PyTaskExecutorWrapper(IPyTaskExecutor& taskExecutor) :
		taskExecutor(taskExecutor)
	{

	}

	void PyTaskExecutorWrapper::execute(const JsonObject& data, const TaskExecutorContext& context)
	{
		py::module_ json = py::module_::import("json");
		std::ostringstream stream;

		stream << data;

		py::dict temp = json.attr("loads")(stream.str().data()).cast<py::dict>();

		taskExecutor.execute(temp, context);
	}
}
