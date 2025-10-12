#include "PythonExecutor.h"

#ifdef __WITH_PYTHON_EXECUTORS__

namespace py = pybind11;

namespace framework
{
	PythonExecutor::PythonExecutor(void* implementation) :
		implementation(static_cast<py::object*>(implementation))
	{

	}

	void PythonExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		

		py::gil_scoped_acquire gil;

		implementation->attr("do_get")(request.getImplementation(), response.getImplementation());
	}

	PythonExecutor::~PythonExecutor()
	{
		py::gil_scoped_acquire gil;

		delete implementation;
	}
}

#endif
