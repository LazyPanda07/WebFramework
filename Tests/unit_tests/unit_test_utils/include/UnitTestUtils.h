#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <format>
#include <filesystem>

#include <ConsoleArgumentParser.h>

#include "ProcessWrapper.h"

namespace unit_test_utils
{
	namespace concepts
	{
		template<typename T>
		concept to_string_invocable = requires(T && value)
		{
			{ std::to_string(value) } -> std::same_as<std::string>;
		};
	}
}

namespace unit_test_utils
{
	template<typename... Args>
	std::vector<std::string> splitArguments(const std::string& arguments, Args&&... args);

	template<typename T>
	std::string convertToString(T&& value);

	template<typename... Args>
	std::string concat(Args&&... args);
}

namespace unit_test_utils
{
	template<typename... Args>
	std::vector<std::string> splitArguments(const std::string& arguments, Args&&... args)
	{
		std::istringstream stream(arguments);
		std::vector<std::string> result;

		while (true)
		{
			std::string temp;

			stream >> temp;

			if (temp.empty())
			{
				break;
			}

			result.emplace_back(std::move(temp));
		}

		(result.emplace_back(convertToString(std::forward<Args>(args))), ...);

		return result;
	}

	template<typename T>
	std::string convertToString(T&& value)
	{
		using ActualT = std::remove_cvref_t<decltype(value)>;

		if constexpr (std::same_as<std::filesystem::path, ActualT>)
		{
			return value.string();
		}
		else if constexpr (concepts::to_string_invocable<ActualT>)
		{
			return std::to_string(value);
		}
		else if constexpr (std::convertible_to<T, std::string>)
		{
			return std::string(value);
		}
		else if constexpr (std::same_as<std::vector<std::string>, ActualT>)
		{
			static_assert(false, "Wrong type std::vector<std::string>");
		}
		else
		{
			throw std::runtime_error(std::format("Wrong type: {}", typeid(T).name()));
		}
	}

	template<typename... Args>
	std::string concat(Args&&... args)
	{
		std::string result;

		(result.append(std::format("{} ", convertToString(std::forward<Args>(args)))), ...);

		return result;
	}
}
