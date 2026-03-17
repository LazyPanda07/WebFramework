#pragma once

#include <string_view>

namespace framework
{
	/**
	 * @brief Uses first element of databaseImplementation
	 */
	struct DefaultDatabase
	{
		static inline constexpr std::string_view databaseImplementationName = "";
	};
}
