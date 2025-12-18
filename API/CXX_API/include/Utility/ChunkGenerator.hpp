#pragma once

#include <string>

namespace framework::utility
{
	class ChunkGenerator
	{
	public:
		ChunkGenerator() = default;

		virtual std::string_view generate(size_t& size) = 0;

		virtual ~ChunkGenerator() = default;
	};
}
