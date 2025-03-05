#pragma once

#include <filesystem>

#include "WebFrameworkCoreConstants.h"

namespace framework
{
	namespace interfaces
	{
		class WEB_FRAMEWORK_CORE_API IFile
		{
		public:
			virtual const std::filesystem::path& getPathToAssets() const = 0;

			virtual ~IFile() = default;
		};
	}
}
