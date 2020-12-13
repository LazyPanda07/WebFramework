#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		class MissingLoadTypeException : public BaseExecutorException
		{
		public:
			MissingLoadTypeException(const std::string& className);

			~MissingLoadTypeException() = default;
		};
	}
}