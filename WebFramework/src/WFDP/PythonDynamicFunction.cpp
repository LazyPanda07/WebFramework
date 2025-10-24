#include "PythonDynamicFunction.h"

namespace py = pybind11;

namespace framework
{
	PythonDynamicFunction::PythonDynamicFunction(void* cls)
	{
		py::gil_scoped_acquire gil;

		functor = (*static_cast<py::type*>(cls))();
	}

	std::string PythonDynamicFunction::operator ()(const std::vector<std::string>& arguments) const
	{
		py::gil_scoped_acquire gil;
		py::tuple args(arguments.size());

		for (size_t i = 0; i < arguments.size(); i++)
		{
			args[i] = arguments[i];
		}
		
		return functor(*args).cast<std::string>();
	}
}
