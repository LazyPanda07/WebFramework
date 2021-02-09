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
