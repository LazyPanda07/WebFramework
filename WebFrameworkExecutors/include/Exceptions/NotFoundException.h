#pragma once

#include "Exceptions/ExecutorException.h"

namespace framework::exceptions
{
	/// <summary>
	/// HTTP not found 404 error code
	/// </summary>
	class  NotFoundException : public ExecutorException
	{
	public:
		NotFoundException(std::string_view additionalInformation = "");

		virtual ~NotFoundException() = default;
	};
}
