#pragma once

#include <filesystem>

#ifdef __LINUX__
using HMODULE = void*;
#else
#include <Windows.h>
#endif

namespace framework::utility
{
	enum class LoadSourceType
	{
		dynamicLibrary,
		python
	};

	std::string makePathToLoadSource(const std::filesystem::path& pathToSource, LoadSourceType& type);

	std::string getPathToWebFrameworkSharedLibrary();
}
