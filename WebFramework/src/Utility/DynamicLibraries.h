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
		python,
		dynamicLibrary,
		dotNet
	};

	std::string makePathToLoadSource(const std::filesystem::path& pathToSource, LoadSourceType& type);

	std::string getPathToWebFrameworkSharedLibrary();

	HMODULE loadLibrary(const std::filesystem::path& pathToLibrary);
}
