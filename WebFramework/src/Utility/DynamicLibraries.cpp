#include "DynamicLibraries.h"

#include <format>

#include "Exceptions/FileDoesNotExistException.h"

#ifdef __LINUX__
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#endif

using namespace std;

std::string getCurrentSharedLibraryPath() {
#if defined(_WIN32)
	HMODULE hModule = NULL;
	// Get handle to this module by using the address of this function
	if (GetModuleHandleExA(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&getCurrentSharedLibraryPath,
		&hModule)) {
		char path[MAX_PATH];
		DWORD size = GetModuleFileNameA(hModule, path, MAX_PATH);
		if (size > 0) {
			return std::string(path, size);
		}
	}
	return std::string();

#elif defined(__linux__)
	Dl_info info;
	if (dladdr((void*)&getCurrentSharedLibraryPath, &info) && info.dli_fname) {
		char fullpath[PATH_MAX];
		if (realpath(info.dli_fname, fullpath)) {
			return std::string(fullpath);
		}
		else {
			return std::string(info.dli_fname); // fallback
		}
	}
	return std::string();
#endif
}

namespace framework::utility
{
	string makePathToDynamicLibrary(const filesystem::path& pathToSource)
	{
		if (pathToSource.has_extension())
		{
			return pathToSource.string();
		}

#ifdef __LINUX__
		filesystem::path temp(pathToSource);
		filesystem::path parent = temp.parent_path();
		filesystem::path fileName = temp.filename();

		return format("{}/lib{}.so", parent.string(), fileName.string());
#else
		return format("{}.dll", pathToSource.string());
#endif
	}

	string getPathToWebFrameworkSharedLibrary()
	{
#ifdef __LINUX__
		Dl_info info;

		if (dladdr(reinterpret_cast<void*>(&getPathToWebFrameworkSharedLibrary), &info) && info.dli_fname)
		{
			char fullpath[PATH_MAX]{};

			return realpath(info.dli_fname, fullpath) ?
				fullpath :
				info.dli_fname;
		}
#else
		HMODULE hModule = nullptr;
		BOOL result = GetModuleHandleExA
		(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			reinterpret_cast<LPCSTR>(&getPathToWebFrameworkSharedLibrary),
			&hModule
		);

		if (result)
		{
			char path[MAX_PATH]{};

			if (GetModuleFileNameA(hModule, path, MAX_PATH))
			{
				return path;
			}
		}
#endif // __LINUX__

		return "";
	}
}
