#pragma once

#include "DatabaseException.h"

namespace framework
{
	namespace exceptions
	{
		/// @brief Class for SQLite3 exceptions
		class SQLite3Exception : public DatabaseException
		{
		public:
			SQLite3Exception(const std::string& errorMessage, std::string_view query = "");

			~SQLite3Exception() = default;
		};
	}
}
