#pragma once

#include <string_view>

namespace framework
{
	/**
	 * @brief Uses SQLite3 database
	 */
	struct SqliteDatabase
	{
		static inline constexpr std::string_view databaseImplementationName = "sqlite";
	};
}
