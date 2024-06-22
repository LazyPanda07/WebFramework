#pragma once

#include "Import/WebFrameworkCore.h"

namespace framework
{
	namespace utility
	{
		class ChunkGenerator
		{
		public:
			ChunkGenerator() = default;

			/**
			 * @brief Generates chunk. Finish generation with empty string
			 * @return 
			 */
			virtual std::string generate() = 0;

			virtual ~ChunkGenerator() = default;
		};
	}
}
