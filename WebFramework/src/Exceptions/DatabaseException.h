#pragma once

#include "BaseWebFrameworkException.h"

namespace framework
{
	namespace exceptions
	{
		/// @brief Base class for all database exceptions
		class WEB_FRAMEWORK_API DatabaseException : public BaseWebFrameworkException
		{
		public:
			DatabaseException(std::string_view errorMessage);

			virtual ~DatabaseException() = default;
		};
	}
}
