#pragma once

#include <Utility/ChunkGenerator.hpp>

namespace framework::utility
{
	class PyChunkGenerator : public ChunkGenerator
	{
	public:
		PyChunkGenerator() = default;

		std::string_view generate() override;

		~PyChunkGenerator() = default;
	};
}
