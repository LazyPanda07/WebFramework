#include "PythonRuntime.h"

#include <Log.h>

#ifdef __WITH_PYTHON_EXECUTORS__

namespace py = pybind11;

namespace framework::runtime
{
	void PythonRuntime::loadSymbols()
	{
#ifdef __LINUX__
		py::object sysconfig = py::module_::import("sysconfig");
		std::string result = sysconfig.attr("get_config_var")("LDLIBRARY").cast<std::string>();

		dlopen(result.data(), RTLD_NOW | RTLD_GLOBAL);
#endif
	}

	PythonRuntime::PythonRuntime() :
		called(false)
	{
		py::initialize_interpreter();

		py::gil_scoped_acquire gil;

		PythonRuntime::loadSymbols();

		if (Log::isValid())
		{
			Log::info("Initialize Python interpreter", "LogRuntime");
		}

		api = py::module_::import("web_framework_api");
	}

	void PythonRuntime::initializeWebFramework(std::string_view libraryPath)
	{
		if (called)
		{
			return;
		}

		py::gil_scoped_acquire gil;

		if (py::hasattr(api, "initialize_web_framework"))
		{
			api.attr("initialize_web_framework")(libraryPath.data());

			called = true;
		}
		else
		{
			throw std::runtime_error("Can't find initialize_web_framework function");
		}
	}

	PythonRuntime::~PythonRuntime()
	{
		py::finalize_interpreter();
	}
}
#endif
