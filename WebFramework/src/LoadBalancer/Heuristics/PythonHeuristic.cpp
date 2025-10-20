#include "PythonHeuristic.h"

#include <Log.h>

#ifdef __WITH_PYTHON_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/PythonRuntime.h"

namespace py = pybind11;

namespace framework::load_balancer
{
	PythonHeuristic::PythonHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, const utility::LoadSource& source) :
		ip(ip),
		port(port),
		useHTTPS(useHTTPS),
		implementation(nullptr)
	{
		std::any temp = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>().getClass(heuristicName.data(), source);

		if (!temp.has_value())
		{
			if (Log::isValid())
			{
				Log::fatalError("Can't find {}", "LogPythonHeuristic", 4, heuristicName);
			}

			throw std::runtime_error(std::format("Can't find {}", heuristicName));
		}

		implementation = new py::object(std::any_cast<py::object>(temp)(ip.data(), port.data(), useHTTPS));
	}

	uint64_t PythonHeuristic::operator ()() const
	{
		py::gil_scoped_acquire gil;

		return implementation->attr("__call__")().cast<uint64_t>();
	}

	void PythonHeuristic::onStart()
	{
		py::gil_scoped_acquire gil;

		implementation->attr("on_start")();
	}

	void PythonHeuristic::onEnd()
	{
		py::gil_scoped_acquire gil;

		implementation->attr("on_end")();
	}

	const std::string& PythonHeuristic::getIp() const
	{
		return ip;
	}

	const std::string& PythonHeuristic::getPort() const
	{
		return port;
	}

	bool PythonHeuristic::getUseHTTPS() const
	{
		return useHTTPS;
	}

	PythonHeuristic::~PythonHeuristic()
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
