#pragma once

#include <string>

#include "WebFrameworkCoreConstants.h"

namespace framework::utility
{
	class WEB_FRAMEWORK_CORE_API ChunkGenerator
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
