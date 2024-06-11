#pragma once

#include "BaseWebFrameworkException.h"

namespace framework
{
	namespace exceptions
	{
		/// @brief Base class for all database exceptions
		class DatabaseException : public BaseWebFrameworkException
		{
		public:
			DatabaseException(const std::string& errorMessage);

			virtual ~DatabaseException() = default;
		};
	}
}
