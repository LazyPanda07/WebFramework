#include "DynamicLibraries.h"

#include <format>

#include <Exceptions/FileDoesNotExistException.h>
#include <Strings.h>

#ifdef __LINUX__
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#endif

namespace framework::utility
{
	std::string makePathToDynamicLibrary(const std::filesystem::path& pathToSource)
	{
		std::filesystem::path absolutePath = std::filesystem::absolute(pathToSource);
		std::string fileName = absolutePath.filename().string();
		std::string extension;
		std::string prefix;

		if (!absolutePath.has_extension())
		{
#ifdef __LINUX__
			extension = ".so";
#else
			extension = ".dll";
#endif
		}

		absolutePath.remove_filename();

#ifdef __LINUX__
		if (fileName.find("lib") == string::npos)
		{
			prefix = "lib";
		}
#endif

		return format("{}{}{}{}", absolutePath.string(), prefix, fileName, extension);
	}

	std::string getPathToWebFrameworkSharedLibrary()
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
		HMODULE module = nullptr;

		BOOL result = GetModuleHandleExA
		(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			reinterpret_cast<LPCSTR>(&getPathToWebFrameworkSharedLibrary),
			&module
		);

		if (result)
		{
			char path[MAX_PATH]{};

			if (GetModuleFileNameA(module, path, MAX_PATH))
			{
				return ::utility::strings::replaceAll(path, ".DLL", ".dll");
			}
		}
#endif

		return "";
	}
}
