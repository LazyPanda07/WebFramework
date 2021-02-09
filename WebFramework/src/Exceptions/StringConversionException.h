#pragma once

#if defined(EXCEPTIONS_DLL) || defined(CONVERSION_DLL) || defined(ALL_DLL)
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
