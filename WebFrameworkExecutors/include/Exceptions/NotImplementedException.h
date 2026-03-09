#pragma once

#include "Exceptions/NotFoundException.h"

namespace framework::exceptions
{
	/// <summary>
	/// Default exception for all HTTP methods in Executor
	/// <para>Server throws 404 error</para>
	/// </summary>
	class EXECUTORS_API NotImplementedException : public NotFoundException
	{
	public:
		NotImplementedException(std::string_view className, std::string_view methodName);

		~NotImplementedException() = default;
	};
}
