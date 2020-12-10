#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		class CantFindFunctionException : public BaseExecutorException
		{
		public:
			CantFindFunctionException();

			~CantFindFunctionException() = default;
		};
	}
}