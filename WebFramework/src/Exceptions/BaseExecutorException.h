#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Base class for most of WebFramework exceptions
		/// </summary>
		class BaseExecutorException : public std::runtime_error
		{
		public:
			BaseExecutorException(const std::string& message);

			~BaseExecutorException() = default;
		};
	}
}
