#pragma once

#include "WebFrameworkCore.h"

namespace framework
{
	namespace utility
	{
		class WEB_FRAMEWORK_API ChunkGenerator
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
