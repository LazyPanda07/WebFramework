#pragma once

#include "Exceptions/ExecutorException.h"

#include "Framework/WebFrameworkPlatform.h"

namespace framework::exceptions
{
	/// <summary>
	/// Missing LoadType in .json file of executor
	/// </summary>
	class WEB_FRAMEWORK_API MissingLoadTypeException : public ExecutorException
	{
	public:
		MissingLoadTypeException(std::string_view className);

		~MissingLoadTypeException() = default;
	};
}
