#include "ExecutorsUtility.h"

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
	ExecutorAPIType getExecutorAPIType(std::string_view name)
	{
		constexpr std::array<std::string_view, static_cast<size_t>(ExecutorAPIType::count)> types =
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
				return static_cast<ExecutorAPIType>(i);
			}
		}

		throw std::runtime_error(std::format("Wrong executor type name: {}", name));

		return ExecutorAPIType::count;
	}
}
