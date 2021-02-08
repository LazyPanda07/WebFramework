#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		class BadRequestException : public BaseExecutorException
		{
		public:
			BadRequestException(const std::string& additionalInformation = "");
		};
	}
}
