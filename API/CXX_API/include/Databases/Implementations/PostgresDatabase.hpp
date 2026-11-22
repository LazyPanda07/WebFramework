#pragma once

#include <string_view>

namespace framework
{
	/**
	 * @brief Uses Postgres database
	 */
	struct PostgresDatabase
	{
		static inline constexpr std::string_view databaseImplementationName = "postgres";
	};
}
