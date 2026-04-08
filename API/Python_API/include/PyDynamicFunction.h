#pragma once

#include <string>

#include <pybind11/pybind11.h>

namespace framework
{
	class IDynamicFunction
	{
	public:
		IDynamicFunction() = default;

		virtual std::string operator ()(pybind11::dict arguments) const = 0;

		virtual ~IDynamicFunction() = default;
	};

	class PyDynamicFunction : public IDynamicFunction
	{
	public:
		static constexpr std::string_view dynamicFunctionImplementationName = "python";

	public:
		PyDynamicFunction() = default;

		std::string operator ()(pybind11::dict arguments) const override;

		~PyDynamicFunction() = default;
	};
}
