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
		if (!Py_IsInitialized())
		{
			guard = std::make_unique<py::scoped_interpreter>();

			if (Log::isValid())
			{
				Log::info("Initialize Python interpreter", "LogRuntime");
			}
		}
		else if (Log::isValid())
		{
			Log::info("Python interpreter already initialized", "LogRuntime");
		}

		PythonRuntime::loadSymbols();

		py::gil_scoped_acquire gil; // load internals

		api = py::module_::import("web_framework_api");
	}

	PythonRuntime::PythonRuntime(PythonRuntime&& other) noexcept
	{
		(*this) = std::move(other);
	}

	PythonRuntime& PythonRuntime::operator =(PythonRuntime&& other) noexcept
	{
		guard = std::move(other.guard);
		api = other.api;
		called = other.called;

		other.called = false;

		return *this;
	}

	void PythonRuntime::finishInitialization()
	{
		if (guard)
		{
			PyEval_SaveThread();
		}
	}

	void* PythonRuntime::createExecutorSettings(const void* implementation) const
	{
		py::object cls = api.attr("ExecutorSettings");

		return new py::object(cls(reinterpret_cast<uint64_t>(implementation)));
	}

	void* PythonRuntime::createHTTPRequest(framework::interfaces::IHTTPRequest* request) const
	{
		py::object cls = api.attr("HTTPRequest");

		return new py::object(cls(reinterpret_cast<uint64_t>(request)));
	}

	void* PythonRuntime::createHTTPResponse(framework::interfaces::IHTTPResponse* response) const
	{
		py::object cls = api.attr("HTTPResponse");

		return new py::object(cls(reinterpret_cast<uint64_t>(response)));
	}

	void PythonRuntime::initializeWebFramework(std::string_view libraryPath)
	{
		if (called)
		{
			return;
		}

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
}
#endif
