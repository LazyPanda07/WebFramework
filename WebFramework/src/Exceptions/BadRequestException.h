#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		class WEB_FRAMEWORK_API BadRequestException : public BaseExecutorException
		{
		public:
			BadRequestException(const std::string& additionalInformation = "");
		};
	}
}
