#pragma once

#include <pybind11/pybind11.h>

namespace registrar
{
	void registerTaskBrokers(pybind11::module_& m);
}
