#include "Registrars/LoadBalancerRegistrar.h"

#include "LoadBalancer/PyLoadBalancerHeuristic.h"

namespace py = pybind11;

namespace registrar
{
	void registerLoadBalancerHeuristic(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::LoadBalancerHeuristic, framework::PyLoadBalancerHeuristic>(m, "LoadBalancerHeuristic")
			.def(py::init<std::string_view, std::string_view, bool>(), "ip"_a, "port"_a, "use_https"_a)
			.def("get_ip", &framework::LoadBalancerHeuristic::getIp)
			.def("get_port", &framework::LoadBalancerHeuristic::getPort)
			.def("get_use_https", &framework::LoadBalancerHeuristic::getUseHTTPS)
			.def("on_start", &framework::LoadBalancerHeuristic::onStart)
			.def("on_end", &framework::LoadBalancerHeuristic::onEnd)
			.def("__call__", &framework::LoadBalancerHeuristic::operator ());
	}
}
