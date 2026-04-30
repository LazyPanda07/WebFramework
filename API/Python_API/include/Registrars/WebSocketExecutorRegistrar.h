#pragma once

#include <pybind11/pybind11.h>

namespace registrar
{
	void registerWebSocketExecutor(pybind11::module_& m);
}
