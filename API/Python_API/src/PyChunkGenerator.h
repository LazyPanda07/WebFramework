#pragma once

#include <Utility/ChunkGenerator.hpp>

namespace framework::utility
{
	class PyChunkGenerator : public ChunkGenerator
	{
	public:
		std::string_view generate() override;
	};
}
