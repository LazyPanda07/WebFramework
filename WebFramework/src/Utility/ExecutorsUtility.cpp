#include "Utility/ExecutorsUtility.h"

#include <array>
#include <stdexcept>
#include <format>

#include "ExecutorsConstants.h"

static constexpr bool equals(std::string_view first, std::string_view second) noexcept
{
	if (first.size() != second.size())
	{
		return false;
	}

	switch (first.size())
	{
	case 5: if (((first[4] | 0x20) != (second[4] | 0x20))) return false; [[fallthrough]];
	case 6: if (((first[5] | 0x20) != (second[5] | 0x20))) return false; [[fallthrough]];
	case 4: if (((first[3] | 0x20) != (second[3] | 0x20))) return false; [[fallthrough]];
	case 3: if (((first[2] | 0x20) != (second[2] | 0x20))) return false; [[fallthrough]];
	case 2: if (((first[1] | 0x20) != (second[1] | 0x20))) return false; [[fallthrough]];
	case 1: if (((first[0] | 0x20) != (second[0] | 0x20))) return false; [[fallthrough]];
	default: return true;
	}
}

namespace framework::utility
{
	ExecutorApiType getExecutorApiType(std::string_view name)
	{
		constexpr std::array<std::string_view, static_cast<size_t>(ExecutorApiType::count)> types =
		{
			json_settings::ccExecutorKey,
			json_settings::cxxExecutorKey,
			json_settings::pythonExecutorKey,
			json_settings::csharpExecutorKey
		};

		for (size_t i = 0; i < types.size(); i++)
		{
			if (equals(types[i], name))
			{
				return static_cast<ExecutorApiType>(i);
			}
		}

		throw std::runtime_error(std::format("Wrong executor type name: {}", name));

		return ExecutorApiType::count;
	}

	std::function<std::string(const std::vector<std::string>&)> createCxxDynamicFunction(const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result))
	{
		return
			[function, deleter](const std::vector<std::string>& arguments) -> std::string
			{
				const char** temp = new const char* [arguments.size()];

				for (size_t i = 0; i < arguments.size(); i++)
				{
					temp[i] = arguments[i].data();
				}

				const char* tempResult = function(temp, arguments.size());
				std::string result(tempResult);

				if (deleter)
				{
					deleter(const_cast<char*>(tempResult));
				}

				delete[] temp;

				return result;
			};
	}

	bool escapeFromAssets(std::string_view filePath) noexcept
	{
		int depth = 0;

		while (filePath.size())
		{
			size_t i = 0;

			while (i < filePath.size() && filePath[i] != '/')
			{
				i++;
			};

			std::string_view part = filePath.substr(0, i);

			if (part == "..")
			{
				if (!depth)
				{
					return true;
				}

				depth--;
			}
			else if (part.size() && part != ".")
			{
				depth++;
			}

			if (i == filePath.size())
			{
				break;
			}

			filePath.remove_prefix(i + 1);
		}

		return false;
	}
}
