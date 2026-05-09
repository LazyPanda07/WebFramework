#pragma once

#include <stdexcept>
#include <string_view>

#include "WebFrameworkCoreConstants.h"

namespace framework::exceptions
{
	/// @brief Base class for all WebFramework exceptions
	class BaseWebFrameworkException : public std::runtime_error
	{
	public:
		BaseWebFrameworkException(std::string_view errorMessage);

		virtual ~BaseWebFrameworkException() = default;
	};
}
