#pragma once

#include "Import/WebFrameworkCore.h"

namespace framework
{
	namespace interfaces
	{
		class WEB_FRAMEWORK_API IFile
		{
		public:
			virtual const std::filesystem::path& getPathToAssets() const = 0;

			virtual ~IFile() = default;
		};
	}
}
