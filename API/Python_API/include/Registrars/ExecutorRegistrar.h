#pragma once

#include <pybind11/pybind11.h>

namespace registrar
{
	void registerExecutorSettings(pybind11::module_& m);

	void registerExecutors(pybind11::module_& m);
}
