#pragma once

#include <string>
#include <vector>

#include <pybind11/pybind11.h>

namespace framework
{
	class IDynamicFunction
	{
	public:
		IDynamicFunction() = default;

		virtual std::string operator ()(pybind11::args args) const = 0;

		virtual ~IDynamicFunction() = default;
	};

	class PyDynamicFunction : public IDynamicFunction
	{
	public:
		static constexpr std::string_view dynamicFunctionImplementationName = "python";

	public:
		PyDynamicFunction() = default;

		std::string operator ()(pybind11::args args) const override;

		~PyDynamicFunction() = default;
	};
}
