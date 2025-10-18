#include "PyLoadBalancerHeuristic.h"

#include <pybind11/pybind11.h>

namespace framework
{
	PyLoadBalancerHeuristic::PyLoadBalancerHeuristic(std::string_view ip, std::string_view port, bool useHTTPS) :
		LoadBalancerHeuristic(ip, port, useHTTPS)
	{

	}

	uint64_t PyLoadBalancerHeuristic::operator ()() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			uint64_t,
			LoadBalancerHeuristic,
			operator()
		);
	}

	void PyLoadBalancerHeuristic::onStart()
	{
		PYBIND11_OVERRIDE
		(
			void,
			LoadBalancerHeuristic,
			onStart
		);
	}

	void PyLoadBalancerHeuristic::onEnd()
	{
		PYBIND11_OVERRIDE
		(
			void,
			LoadBalancerHeuristic,
			onEnd
		);
	}
}
