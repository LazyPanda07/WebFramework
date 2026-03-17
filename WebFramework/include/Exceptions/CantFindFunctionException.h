#pragma once

#include "Exceptions/ExecutorException.h"

#include "Framework/WebFrameworkPlatform.h"

namespace framework::exceptions
{
	/// <summary>
	/// Can't find CreateExecutorFunction for executor
	/// </summary>
	class WEB_FRAMEWORK_API CantFindFunctionException : public ExecutorException
	{
	public:
		CantFindFunctionException(std::string_view createFunctionName);

		~CantFindFunctionException() = default;
	};
}
