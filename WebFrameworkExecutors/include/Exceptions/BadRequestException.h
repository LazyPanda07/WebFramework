#pragma once

#include "ExecutorException.h"

namespace framework::exceptions
{
	/// <summary>
	/// HTTP bad request 400 error code
	/// </summary>
	class BadRequestException : public ExecutorException
	{
	public:
		BadRequestException(std::string_view additionalInformation = "");

		virtual ~BadRequestException() = default;
	};
}
