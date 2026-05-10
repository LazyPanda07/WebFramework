#pragma once

#include "ExecutorException.h"

namespace framework::exceptions
{
	/// <summary>
	/// HTTP forbidden 403 error code
	/// </summary>
	class ForbiddenException : public ExecutorException
	{
	public:
		ForbiddenException(std::string_view additionalInformation = "");

		virtual ~ForbiddenException() = default;
	};
}
