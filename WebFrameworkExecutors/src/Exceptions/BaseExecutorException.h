#pragma once

#include "Exceptions/BaseWebFrameworkException.h"

#include "ExecutorsConstants.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Base class for most of WebFramework exceptions
		/// </summary>
		class EXECUTORS_API BaseExecutorException : public BaseWebFrameworkException
		{
		public:
			BaseExecutorException(std::string_view message);

			virtual ~BaseExecutorException() = default;
		};
	}
}
