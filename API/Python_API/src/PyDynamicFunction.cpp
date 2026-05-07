#include "PyDynamicFunction.h"

namespace framework
{
	std::string PyDynamicFunction::operator ()(pybind11::dict arguments) const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string,
			IDynamicFunction,
			operator ()
			arguments
		);
	}
}
