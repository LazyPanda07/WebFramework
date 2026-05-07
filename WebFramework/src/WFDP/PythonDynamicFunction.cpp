#include "WFDP/PythonDynamicFunction.h"

#ifdef __WITH_PYTHON_EXECUTORS__

namespace py = pybind11;

namespace framework
{
	PythonDynamicFunction::PythonDynamicFunction(void* cls)
	{
		py::gil_scoped_acquire gil;

		functor = (*static_cast<py::type*>(cls))();
	}

	std::string PythonDynamicFunction::operator ()(const json::JsonObject& arguments) const
	{
		py::gil_scoped_acquire gil;
		py::module_ json = py::module_::import("json");
		
		return functor(json.attr("loads")(static_cast<std::string>(arguments).data()).cast<py::dict>()).cast<std::string>();
	}
}

#endif
