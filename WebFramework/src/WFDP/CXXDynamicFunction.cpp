#include "CXXDynamicFunction.h"

namespace framework
{
	CXXDynamicFunction::CXXDynamicFunction(const std::function<std::string(const std::vector<std::string>&)>& functor) :
		functor(functor)
	{

	}

	CXXDynamicFunction::CXXDynamicFunction(std::function<std::string(const std::vector<std::string>&)>&& functor) noexcept :
		functor(std::move(functor))
	{

	}

	std::string CXXDynamicFunction::operator()(const std::vector<std::string>& arguments) const
	{
		return functor(arguments);
	}
}
