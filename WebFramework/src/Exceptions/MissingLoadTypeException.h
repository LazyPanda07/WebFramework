#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Missing loadType in .xml file of executor
		/// </summary>
		class MissingLoadTypeException : public BaseExecutorException
		{
		public:
			MissingLoadTypeException(const std::string& className);

			~MissingLoadTypeException() = default;
		};
	}
}
