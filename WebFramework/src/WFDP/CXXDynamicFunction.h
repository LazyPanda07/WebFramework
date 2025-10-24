#pragma once

#include "DynamicFunction.h"

#include <functional>

namespace framework
{
	class CXXDynamicFunction : public DynamicFunction
	{
	private:
		std::function<std::string(const std::vector<std::string>&)> functor;

	public:
		CXXDynamicFunction(const std::function<std::string(const std::vector<std::string>&)>& functor);

		CXXDynamicFunction(std::function<std::string(const std::vector<std::string>&)>&& functor) noexcept;

		std::string operator ()(const std::vector<std::string>& arguments) const override;

		~CXXDynamicFunction() = default;
	};
}
