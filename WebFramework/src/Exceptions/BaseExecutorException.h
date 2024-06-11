#pragma once

#include "BaseWebFrameworkException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Base class for most of WebFramework exceptions
		/// </summary>
		class BaseExecutorException : public BaseWebFrameworkException
		{
		public:
			BaseExecutorException(std::string_view message);

			virtual ~BaseExecutorException() = default;
		};
	}
}
