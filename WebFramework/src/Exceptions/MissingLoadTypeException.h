#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		class MissingLoadTypeException : public BaseExecutorException
		{
		public:
			MissingLoadTypeException();

			~MissingLoadTypeException() = default;
		};
	}
}