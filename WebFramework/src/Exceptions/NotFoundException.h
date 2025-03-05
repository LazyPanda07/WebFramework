#pragma once

#include "Exceptions/BaseExecutorException.h"

#include "Import/WebFrameworkConstants.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// HTTP not found 404 error code
		/// </summary>
		class WEB_FRAMEWORK_API NotFoundException : public BaseExecutorException
		{
		public:
			NotFoundException(std::string_view additionalInformation = "");

			virtual ~NotFoundException() = default;
		};
	}
}
