#pragma once

#include "DynamicFunction.h"

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

namespace framework
{
	class PythonDynamicFunction : public DynamicFunction
	{
	private:
		pybind11::object functor;

	public:
		PythonDynamicFunction(void* cls);

		std::string operator ()(const std::vector<std::string>& arguments) const override;

		~PythonDynamicFunction() = default;
	};
}

#endif
