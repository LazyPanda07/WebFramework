#pragma once

#include "Exceptions/BaseExecutorException.h"

#include "Framework/WebFrameworkPlatform.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Missing LoadType in .json file of executor
		/// </summary>
		class WEB_FRAMEWORK_API MissingLoadTypeException : public BaseExecutorException
		{
		public:
			MissingLoadTypeException(std::string_view className);

			~MissingLoadTypeException() = default;
		};
	}
}
