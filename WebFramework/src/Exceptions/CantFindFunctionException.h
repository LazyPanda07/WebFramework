#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Can't find createBaseExecutorSubclassFunction for executor
		/// </summary>
		class CantFindFunctionException : public BaseExecutorException
		{
		public:
			CantFindFunctionException(const std::string& createFunctionName);

			~CantFindFunctionException() = default;
		};
	}
}
