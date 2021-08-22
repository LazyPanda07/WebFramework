#pragma once

#include "headers.h"

namespace framework
{
	namespace exceptions
	{
		/// @brief Base class for all WebFramework exceptions
		class WEB_FRAMEWORK_API BaseWebFrameworkException : public std::runtime_error
		{
		public:
			BaseWebFrameworkException(const std::string& errorMessage);

			virtual ~BaseWebFrameworkException() = default;
		};
	}
}
