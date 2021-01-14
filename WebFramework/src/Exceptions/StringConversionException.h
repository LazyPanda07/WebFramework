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
		/// Errors in UTF8 convertion
		/// </summary>
		class WEB_FRAMEWORK_API StringConversionException : public std::runtime_error
		{
		public:
			StringConversionException();

			~StringConversionException() = default;
		};
	}
}
