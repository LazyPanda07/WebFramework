#pragma once

#include "Exceptions/BaseWebFrameworkException.h"

#include "Import/WebFrameworkConstants.h"

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
			DynamicPagesSyntaxException(std::string_view syntaxError);

			~DynamicPagesSyntaxException() = default;
		};
	}
}
