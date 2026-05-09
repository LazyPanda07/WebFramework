#pragma once

#include <string_view>

#include "WebFrameworkCoreConstants.h"

namespace framework::utility
{
	class ChunkGenerator
	{
	public:
		ChunkGenerator() = default;

		/**
		 * @brief Generates chunk. Finish generation with empty string
		 * @return
		 */
		virtual std::string_view generate() = 0;

		virtual ~ChunkGenerator() = default;
	};
}
