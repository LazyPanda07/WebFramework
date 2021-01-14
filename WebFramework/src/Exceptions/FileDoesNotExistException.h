#pragma once

#include <stdexcept>

#if defined(EXECUTOR_DLL) || defined(SQLITE_DLL) || defined(LOAD_BALANCER_DLL) || defined(CONVERSION_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Wrong file path
		/// </summary>
		class WEB_FRAMEWORK_API FileDoesNotExistException : public std::runtime_error
		{
		public:
			FileDoesNotExistException(const std::string& fileName);

			~FileDoesNotExistException() = default;
		};
	}
}
