#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Missing loadType in .json file of executor
		/// </summary>
		class WEB_FRAMEWORK_API MissingLoadTypeException : public BaseExecutorException
		{
		public:
			MissingLoadTypeException(std::string_view className);

			~MissingLoadTypeException() = default;
		};
	}
}
