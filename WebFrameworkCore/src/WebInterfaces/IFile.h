#pragma once

#include <filesystem>

namespace framework::interfaces
{
	class IFile
	{
	public:
		virtual const std::filesystem::path& getPathToAssets() const = 0;

		virtual ~IFile() = default;
	};
}
