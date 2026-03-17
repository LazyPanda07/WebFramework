#include "TaskBroker/PyTaskExecutor.h"

#include <sstream>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace framework::task_broker
{
	void PyTaskExecutor::operator ()(py::dict data)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			IPyTaskExecutor,
			operator (),
			data
		);
	}

	PyTaskExecutorWrapper::PyTaskExecutorWrapper(IPyTaskExecutor& taskExecutor) :
		taskExecutor(taskExecutor)
	{

	}

	void PyTaskExecutorWrapper::operator ()(const JsonObject& data)
	{
		py::module_ json = py::module_::import("json");
		std::ostringstream stream;

		stream << data;

		py::dict temp = json.attr("loads")(stream.str().data()).cast<py::dict>();

		taskExecutor(temp);
	}
}
