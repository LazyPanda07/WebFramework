#pragma once

#include "BaseWebFrameworkException.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Syntax erros in .wfdp
		/// </summary>
		class WEB_FRAMEWORK_API DynamicPagesSyntaxException : public BaseWebFrameworkException
		{
		public:
			DynamicPagesSyntaxException(const std::string& syntaxError);

			~DynamicPagesSyntaxException() = default;
		};
	}
}
