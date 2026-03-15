#include "LoadBalancer/Heuristics/PythonHeuristic.h"

#include "Utility/Utils.h"

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
		std::optional<py::object> temp = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>().getClass(heuristicName, source);

		if (!temp)
		{
			utility::logAndThrowException<logging::message::cantFindPythonHeuristicSource, logging::category::pythonHeuristic>(heuristicName, py::repr(std::get<py::module_>(source)).cast<std::string>());
		}

		implementation = new py::object((*temp)(ip.data(), port.data(), useHTTPS));
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
