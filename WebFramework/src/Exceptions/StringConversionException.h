#pragma once

#include "BaseWebFrameworkException.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Errors in UTF8 convertion
		/// </summary>
		class WEB_FRAMEWORK_API StringConversionException : public BaseWebFrameworkException
		{
		public:
			StringConversionException();

			~StringConversionException() = default;
		};
	}
}
