#pragma once

#include <type_traits>
#include <string>

namespace framework::utility::concepts
{
	template<typename T>
	concept RouteParameterType = std::same_as<T, std::string> || std::same_as<T, int64_t> || std::same_as<T, double>;
}
