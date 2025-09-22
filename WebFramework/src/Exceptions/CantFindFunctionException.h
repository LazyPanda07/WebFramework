#pragma once

#include "Exceptions/BaseExecutorException.h"

#include "Framework/WebFrameworkPlatform.h"

namespace framework::exceptions
{
	/// <summary>
	/// Can't find CreateExecutorFunction for executor
	/// </summary>
	class WEB_FRAMEWORK_API CantFindFunctionException : public BaseExecutorException
	{
	public:
		CantFindFunctionException(std::string_view createFunctionName);

		~CantFindFunctionException() = default;
	};
}
