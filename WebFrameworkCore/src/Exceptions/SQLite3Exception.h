#pragma once

#include "DatabaseException.h"

namespace framework
{
	namespace exceptions
	{
		/// @brief Class for SQLite3 exceptions
		class WEB_FRAMEWORK_CORE_API SQLite3Exception : public DatabaseException
		{
		public:
			SQLite3Exception(std::string_view errorMessage, std::string_view query = "");

			~SQLite3Exception() = default;
		};
	}
}
