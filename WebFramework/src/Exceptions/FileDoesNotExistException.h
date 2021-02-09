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
