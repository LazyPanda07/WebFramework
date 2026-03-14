#pragma once

#include <pybind11/pybind11.h>

namespace registrar
{
	void registerHttpRequest(pybind11::module_& m);
}
