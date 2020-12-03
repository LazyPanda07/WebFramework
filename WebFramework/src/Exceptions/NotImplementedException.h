#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		class NotImplementedException : public BaseExecutorException
		{
		public:
			NotImplementedException();

			~NotImplementedException() = default;
		};
	}
}