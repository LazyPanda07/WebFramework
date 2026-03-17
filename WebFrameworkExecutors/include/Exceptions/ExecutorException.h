#pragma once

#include "Exceptions/BaseWebFrameworkException.h"

#include "ExecutorsConstants.h"

namespace framework::exceptions
{
	/// <summary>
	/// Base class for most of WebFramework exceptions
	/// </summary>
	class EXECUTORS_API ExecutorException : public BaseWebFrameworkException
	{
	public:
		ExecutorException(std::string_view message);

		virtual ~ExecutorException() = default;
	};
}
