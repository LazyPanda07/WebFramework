#pragma once

#include "BaseExecutorException.h"

namespace framework::exceptions
{
	/// <summary>
	/// HTTP bad request 400 error code
	/// </summary>
	class EXECUTORS_API BadRequestException : public BaseExecutorException
	{
	public:
		BadRequestException(std::string_view additionalInformation = "");

		virtual ~BadRequestException() = default;
	};
}
