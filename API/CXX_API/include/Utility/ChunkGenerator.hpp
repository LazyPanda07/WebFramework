#pragma once

#include <string>

namespace framework::utility
{
	class ChunkGenerator
	{
	public:
		ChunkGenerator() = default;

		virtual std::string_view generate() = 0;

		virtual ~ChunkGenerator() = default;
	};
}
