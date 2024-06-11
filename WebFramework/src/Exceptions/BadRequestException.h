#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// HTTP bad request 400 error code
		/// </summary>
		class BadRequestException : public BaseExecutorException
		{
		public:
			BadRequestException(std::string_view additionalInformation = "");

			virtual ~BadRequestException() = default;
		};
	}
}
