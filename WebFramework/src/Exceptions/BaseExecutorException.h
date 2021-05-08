#pragma once

#include "BaseWebFrameworkException.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Base class for most of WebFramework exceptions
		/// </summary>
		class WEB_FRAMEWORK_API BaseExecutorException : public BaseWebFrameworkException
		{
		public:
			BaseExecutorException(const std::string& message);

			virtual ~BaseExecutorException() = default;
		};
	}
}
