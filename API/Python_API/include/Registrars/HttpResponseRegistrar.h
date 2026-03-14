#pragma once

#include <pybind11/pybind11.h>

namespace registrar
{
	void registerHttpResponse(pybind11::module_& m);
}
