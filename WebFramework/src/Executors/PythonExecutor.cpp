#include "PythonExecutor.h"

#ifdef __WITH_PYTHON_EXECUTORS__

namespace py = pybind11;

namespace framework
{
	PythonExecutor::PythonExecutor(void* implementation) :
		implementation(static_cast<py::object*>(implementation))
	{

	}

	PythonExecutor::~PythonExecutor()
	{
		delete implementation;
	}
}

#endif
