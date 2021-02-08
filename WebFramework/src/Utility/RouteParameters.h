#pragma once

#include <string>
#include <unordered_map>
#include <variant>

namespace framework
{
	namespace utility
	{
		struct RouteParameters
		{
		public:
			static constexpr std::string_view stringType = "string:";
			static constexpr std::string_view integerType = "int:";

		public:
			std::string baseRoute;
			std::unordered_map<std::string, std::variant<std::string, int>> parameters;

			RouteParameters(const std::string& xmlRoute);

			RouteParameters(RouteParameters&& other) noexcept;
		};
	}
}
