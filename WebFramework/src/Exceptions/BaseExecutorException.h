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
		/// Base class for most of WebFramework exceptions
		/// </summary>
		class WEB_FRAMEWORK_API BaseExecutorException : public std::runtime_error
		{
		public:
			BaseExecutorException(const std::string& message);

			~BaseExecutorException() = default;
		};
	}
}
