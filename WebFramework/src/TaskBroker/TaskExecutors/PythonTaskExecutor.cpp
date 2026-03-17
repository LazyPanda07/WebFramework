#include "TaskBroker/TaskExecutors/PythonTaskExecutor.h"

#include <sstream>

#ifdef __WITH_PYTHON_EXECUTORS__

namespace py = pybind11;

namespace framework::task_broker
{
	PythonTaskExecutor::PythonTaskExecutor(pybind11::object* implementation) :
		implementation(implementation)
	{

	}

	void PythonTaskExecutor::operator ()(json::JsonObject& data)
	{
		std::ostringstream stream;

		stream << data;

		py::gil_scoped_acquire gil;
		py::module_ json = py::module_::import("json");

		implementation->attr("__call__")(json.attr("loads")(stream.str().data()).cast<py::dict>());
	}

	PythonTaskExecutor::~PythonTaskExecutor()
	{
		if (implementation)
		{
			py::gil_scoped_acquire gil;

			delete implementation;

			implementation = nullptr;
		}
	}
}

#endif
