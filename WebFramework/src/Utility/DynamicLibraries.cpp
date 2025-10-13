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
	std::string makePathToLoadSource(const std::filesystem::path& pathToSource, LoadSourceType& type)
	{
		std::filesystem::path absolutePath = std::filesystem::absolute(pathToSource);
		std::filesystem::path fileName = absolutePath.filename();
		std::string extension;
		std::string prefix;

#ifdef __LINUX__
		if (fileName.find("lib") == std::string::npos)
		{
			prefix = "lib";
		}
#endif

		if (!absolutePath.has_extension())
		{
#ifdef __LINUX__
			extension = ".so";
#else
			extension = ".dll";
#endif

			type = LoadSourceType::dynamicLibrary;
		}
		else if (absolutePath.extension() == ".py")
		{
			prefix = "";
			absolutePath = "";

			fileName.replace_extension();
			
			type = LoadSourceType::python;
		}

		absolutePath.remove_filename();

		return std::format("{}{}{}{}", absolutePath.string(), prefix, fileName.string(), extension);
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
