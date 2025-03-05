#pragma once

#include <stdexcept>
#include <string_view>

#include "WebFrameworkCoreConstants.h"

namespace framework
{
	namespace exceptions
	{
		/// @brief Base class for all WebFramework exceptions
		class WEB_FRAMEWORK_CORE_API BaseWebFrameworkException : public std::runtime_error
		{
		public:
			BaseWebFrameworkException(std::string_view errorMessage);

			virtual ~BaseWebFrameworkException() = default;
		};
	}
}
