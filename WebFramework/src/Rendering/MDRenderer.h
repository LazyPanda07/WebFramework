#pragma once

#include <string>

namespace framework
{
	class MDRenderer
	{
	public:
		MDRenderer() = default;

		std::string render(std::string_view source) const;

		~MDRenderer() = default;
	};
}
