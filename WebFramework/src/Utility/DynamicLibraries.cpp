#include "DynamicLibraries.h"

#include <format>

#include "Exceptions/FileDoesNotExistException.h"

#ifdef __LINUX__
#include <dlfcn.h>
#endif

using namespace std;

namespace framework
{
	namespace utility
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

		HMODULE load(const filesystem::path& dynamicLibraryPath)
		{
			string realDynamicLibraryPath = makePathToDynamicLibrary(dynamicLibraryPath);
			HMODULE result;

			if (!filesystem::exists(realDynamicLibraryPath))
			{
#ifdef __LINUX__
				realDynamicLibraryPath = format("lib{}.so", dynamicLibraryPath.filename().string());
#else
				realDynamicLibraryPath = format("{}.dll", dynamicLibraryPath.filename().string());
#endif
			}


#ifdef __LINUX__
			result = dlopen(realDynamicLibraryPath.data(), RTLD_LAZY);

#else
			result = LoadLibraryA(realDynamicLibraryPath.data());
#endif

			if (!result)
			{
				string fileName;

#ifdef __LINUX__
				fileName = format("lib{}.so", dynamicLibraryPath.filename().string());
#else
				fileName = format("{}.dll", dynamicLibraryPath.filename().string());
#endif

				throw runtime_error(format("Can't load {} or {}", makePathToDynamicLibrary(dynamicLibraryPath), move(fileName)));
			}

			return result;
		}

		bool unload(HMODULE dynamicLibrary)
		{
#ifdef __LINUX__
			return !dlclose(dynamicLibrary);
#else
			return FreeLibrary(dynamicLibrary);
#endif
		}
	}
}
