#pragma once

#include <stdexcept>

#if defined(EXECUTOR_DLL) || defined(SQLITE_DLL) || defined(LOAD_BALANCER_DLL) || defined(CONVERSION_DLL) || defined(WEB_FRAMEWORK_DLL) || defined(MIDDLEWARE_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Wrong path to loadSource from .ini
		/// </summary>
		class WEB_FRAMEWORK_API CantLoadSourceException : public std::runtime_error
		{
		public:
			CantLoadSourceException(const std::string& source);

			~CantLoadSourceException() = default;
		};
	}
}
