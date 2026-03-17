#pragma once

#include <pybind11/pybind11.h>

namespace registrar
{
	void registerUtility(pybind11::module_& m);

	void registerUtilityStructures(pybind11::module_& m);
}
