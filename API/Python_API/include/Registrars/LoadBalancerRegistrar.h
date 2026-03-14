#pragma once

#include <pybind11/pybind11.h>

namespace registrar
{
	void registerLoadBalancerHeuristic(pybind11::module_& m);
}
