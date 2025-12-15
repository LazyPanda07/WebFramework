#include "PythonRuntime.h"

#include <Log.h>

#ifdef __WITH_PYTHON_EXECUTORS__

#include "Executors/PythonExecutor.h"

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

	std::optional<py::object> PythonRuntime::getClass(std::string_view className, const utility::LoadSource& source) const
	{
		const py::module_& module = std::get<py::module_>(source);

		if (!py::hasattr(module, className.data()))
		{
			return std::nullopt;
		}

		py::object cls = module.attr(className.data());

		if (!py::isinstance<py::type>(cls))
		{
			return std::nullopt;
		}

		return cls;
	}

	void PythonRuntime::finishInitialization()
	{
		if (guard)
		{
			PyEval_SaveThread();
		}
	}

	bool PythonRuntime::loadExecutor(std::string_view name, const utility::LoadSource& source)
	{
		if (!std::holds_alternative<py::module_>(source))
		{
			return false;
		}

		py::gil_scoped_acquire gil;
		const py::module_& module = std::get<py::module_>(source);
		std::optional<py::object> cls = this->getClass(name, module);

		if (!cls)
		{
			return false;
		}

		if (Log::isValid())
		{
			Log::info("Found {} in {}", "LogWebFrameworkInitialization", name, py::repr(module).cast<std::string>());
		}

		classes.emplace(name, *cls);

		return true;
	}

	std::unique_ptr<BaseExecutor> PythonRuntime::createExecutor(std::string_view name) const
	{
		auto it = classes.find(name);

		if (it == classes.end())
		{
			throw std::runtime_error(std::format("Can't find executor with name {}", name));
		}

		py::gil_scoped_acquire gil;

		const auto& [_, cls] = *it;

		return std::make_unique<PythonExecutor>(new py::object(cls()));
	}

	void* PythonRuntime::createExecutorSettings(const void* implementation) const
	{
		py::object cls = api.attr("ExecutorSettings");

		return new py::object(cls(reinterpret_cast<uint64_t>(implementation)));
	}

	void* PythonRuntime::createHTTPRequest(framework::interfaces::IHTTPRequest* request) const
	{
		py::object cls = api.attr("HttpRequest");

		return new py::object(cls(reinterpret_cast<uint64_t>(request)));
	}

	void* PythonRuntime::createHTTPResponse(framework::interfaces::IHTTPResponse* response) const
	{
		py::object cls = api.attr("HttpResponse");

		return new py::object(cls(reinterpret_cast<uint64_t>(response)));
	}

	void PythonRuntime::initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath)
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

	std::optional<std::string> PythonRuntime::loadSource(std::string_view pathToSource, utility::LoadSource& source)
	{
		try
		{
			std::filesystem::path pythonSourcePath(pathToSource);
			py::module_ sys = py::module_::import("sys");

			sys.attr("path").attr("append")(pythonSourcePath.parent_path().string().data());

			source = py::module_::import(pythonSourcePath.filename().string().data());
		}
		catch (const py::error_already_set& e)
		{
			return e.what();
		}

		return std::nullopt;
	}
}

#endif
