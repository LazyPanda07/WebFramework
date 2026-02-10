#pragma once

#include <string_view>

namespace framework
{
	/**
	 * @brief Uses Redis database
	 */
	struct RedisDatabase
	{
		static inline constexpr std::string_view databaseImplementationName = "redis";
	};
}
