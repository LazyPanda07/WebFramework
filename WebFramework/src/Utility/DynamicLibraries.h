#pragma once

#include <filesystem>

#ifdef __LINUX__
using HMODULE = void*;
#else
#include <Windows.h>
#endif

namespace framework
{
	namespace utility
	{
		std::string makePathToDynamicLibrary(const std::filesystem::path& pathToSource);

		/**
		 * @brief Load dynamic library by absolute path or name
		 * @param dynamicLibraryPath Path to dynamic library without extension or OS specific prefix
		 * @return 
		 */
		HMODULE load(const std::filesystem::path& dynamicLibraryPath);

		bool unload(HMODULE dynamicLibrary);
	}
}
