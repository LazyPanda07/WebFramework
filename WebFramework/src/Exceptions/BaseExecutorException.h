#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		class BaseExecutorException : public std::runtime_error
		{
		public:
			BaseExecutorException(const std::string& message);

			~BaseExecutorException() = default;
		};
	}
}