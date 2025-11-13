#pragma once

#include "WebFrameworkAPIException.hpp"

#include <format>

namespace framework::exceptions
{
	class NotImplementedDoMethodException : public WebFrameworkAPIException
	{
	public:
		NotImplementedDoMethodException(std::string_view methodName, std::string_view className);

		~NotImplementedDoMethodException() = default;
	};

	inline NotImplementedDoMethodException::NotImplementedDoMethodException(std::string_view methodName, std::string_view className) :
		WebFrameworkAPIException(std::format("Not implemented method {} in {}", methodName, className), ResponseCodes::badRequest)
	{

	}
}
