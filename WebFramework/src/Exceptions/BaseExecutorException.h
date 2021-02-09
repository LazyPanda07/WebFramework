#pragma once

#if defined(EXCEPTIONS_DLL) || defined(ALL_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // EXCEPTIONS_DLL

#include <stdexcept>

#include "WebFrameworkConstants.h"

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
