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
		/// Syntax erros in .wfdp
		/// </summary>
		class WEB_FRAMEWORK_API DynamicPagesSyntaxException : public std::runtime_error
		{
		public:
			DynamicPagesSyntaxException(const std::string& syntaxError);

			~DynamicPagesSyntaxException() = default;
		};
	}
}
