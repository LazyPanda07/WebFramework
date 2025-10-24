#include "PyDynamicFunction.h"

namespace framework
{
	std::string PyDynamicFunction::operator ()(pybind11::args args) const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string,
			IDynamicFunction,
			args
		);
	}
}
