#pragma once

#include "BaseWebFrameworkException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Wrong path to loadSource from .ini
		/// </summary>
		class WEB_FRAMEWORK_API CantLoadSourceException : public BaseWebFrameworkException
		{
		public:
			CantLoadSourceException(const std::string& source);

			~CantLoadSourceException() = default;
		};
	}
}
