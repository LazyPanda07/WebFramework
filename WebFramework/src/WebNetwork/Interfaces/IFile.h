#pragma once

#include "WebFrameworkCore.h"

namespace framework
{
	namespace interfaces
	{
		class IFile
		{
		public:
			virtual const std::filesystem::path& getPathToAssets() const = 0;

			virtual ~IFile() = default;
		};
	}
}
