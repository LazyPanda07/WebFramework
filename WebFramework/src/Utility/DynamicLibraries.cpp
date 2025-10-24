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
		bool hasExtension = absolutePath.has_extension();
		std::string extension;
		std::string prefix;
		auto initPy = [&]()
			{
				extension = "";
				prefix = "";

				fileName.replace_extension();

				type = LoadSourceType::python;
			};
		auto generatePath = [&](std::string_view extension) -> std::string
			{
				return std::filesystem::path(std::format("{}/{}{}{}", absolutePath.string(), prefix, fileName.string(), extension)).make_preferred().string();
			};

		if (hasExtension)
		{
			extension = absolutePath.extension().string();

			if (extension == ".py")
			{
				initPy();
			}
			else
			{
#ifdef __LINUX__
				if (fileName.string().find("lib") == std::string::npos)
				{
					prefix = "lib";
				}
#endif

				type = LoadSourceType::dynamicLibrary;
			}
		}

		absolutePath = absolutePath.parent_path();

		if (!hasExtension)
		{
			if (std::filesystem::exists(generatePath(".py")))
			{
				initPy();
			}
			else
			{
#ifdef __LINUX__
				if (fileName.string().find("lib") == std::string::npos)
				{
					prefix = "lib";
				}

				extension = ".so";
#else
				extension = ".dll";
#endif
				type = LoadSourceType::dynamicLibrary;
			}
		}

		return generatePath(extension);
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
