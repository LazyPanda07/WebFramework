#pragma once

#include <stdexcept>

#if defined(EXCEPTIONS_DLL) || defined(ALL_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // EXCEPTIONS_DLL

namespace framework
{
	namespace exceptions
	{
		/// @brief Base class for all WebFramework exceptions
		class WEB_FRAMEWORK_API BaseWebFrameworkException : public std::runtime_error
		{
		public:
			BaseWebFrameworkException(const std::string& errorMessage);

			virtual ~BaseWebFrameworkException() = default;
		};
	}
}
