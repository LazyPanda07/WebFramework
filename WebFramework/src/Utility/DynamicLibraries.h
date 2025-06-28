#pragma once

#include <filesystem>

#ifdef __LINUX__
using HMODULE = void*;
#else
#include <Windows.h>
#endif

namespace framework::utility
{
	std::string makePathToDynamicLibrary(const std::filesystem::path& pathToSource);

	std::string getPathToWebFrameworkSharedLibrary();
}
