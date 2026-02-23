#pragma once

#include "ExecutorException.h"

namespace framework::exceptions
{
	/// <summary>
	/// HTTP bad request 400 error code
	/// </summary>
	class EXECUTORS_API BadRequestException : public ExecutorException
	{
	public:
		BadRequestException(std::string_view additionalInformation = "");

		virtual ~BadRequestException() = default;
	};
}
