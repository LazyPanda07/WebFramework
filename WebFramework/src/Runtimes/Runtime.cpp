#include "Runtimes/Runtime.h"

#include "Utility/DynamicLibraries.h"

namespace framework::runtime
{
	void Runtime::initializeWebFramework(const utility::LoadSource& source)
	{
#ifdef __ANDROID__
		std::string webFrameworkSharedLibraryPath = "libWebFramework.so";
#else
		std::string webFrameworkSharedLibraryPath = utility::getPathToWebFrameworkSharedLibrary();
#endif

		this->initializeWebFramework(source, webFrameworkSharedLibraryPath);
	}
}
