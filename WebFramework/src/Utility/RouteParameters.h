#pragma once

#include "Import/WebFrameworkCore.h"

namespace framework
{
	namespace utility
	{
		struct RouteParameters
		{
		public:
			static constexpr std::string_view stringType = "string:";
			static constexpr std::string_view integerType = "int:";
			static constexpr std::string_view doubleType = "double:";

			enum class routeParametersType
			{
				stringTypeIndex,
				integerTypeIndex,
				doubleTypeIndex
			};

		public:
			std::string baseRoute;
			std::unordered_map<std::string, std::variant<std::string, int64_t, double>> parameters;
			std::vector<std::string> indices;

			RouteParameters(const std::string& jsonRoute);

			RouteParameters(RouteParameters&& other) noexcept = default;

			RouteParameters& operator =(RouteParameters&& other) noexcept = default;
		};
	}
}
