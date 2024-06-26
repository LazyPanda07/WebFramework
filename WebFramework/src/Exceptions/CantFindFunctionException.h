#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Can't find createExecutorFunction for executor
		/// </summary>
		class WEB_FRAMEWORK_API CantFindFunctionException : public BaseExecutorException
		{
		public:
			CantFindFunctionException(std::string_view createFunctionName);

			~CantFindFunctionException() = default;
		};
	}
}
