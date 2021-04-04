#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// HTTP bad request 400 error code
		/// </summary>
		class WEB_FRAMEWORK_API BadRequestException : public BaseExecutorException
		{
		public:
			BadRequestException(const std::string& additionalInformation = "");
		};
	}
}
