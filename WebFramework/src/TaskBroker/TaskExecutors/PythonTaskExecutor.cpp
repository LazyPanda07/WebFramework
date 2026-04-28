#include "TaskBroker/TaskExecutors/PythonTaskExecutor.h"

#include <sstream>

#ifdef __WITH_PYTHON_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/PythonRuntime.h"

namespace py = pybind11;

namespace framework::task_broker
{
	PythonTaskExecutor::PythonTaskExecutor(pybind11::object* implementation) :
		implementation(implementation)
	{

	}

	void PythonTaskExecutor::execute(json::JsonObject& data, TaskExecutorContext& context)
	{
		std::ostringstream stream;

		stream << data;

		py::gil_scoped_acquire gil;
		py::module_ json = py::module_::import("json");
		std::unique_ptr<py::object> contextWrapper
		(
			runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>().createTaskExecutorContext(&context)
		);

		implementation->attr("execute")(json.attr("loads")(stream.str().data()).cast<py::dict>(), *contextWrapper);
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
