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
	}
}
