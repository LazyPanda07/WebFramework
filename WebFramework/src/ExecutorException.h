#pragma once

#include <exception>

namespace framework
{
	namespace exceptions
	{
		class ExecutorException : public std::exception
		{
		public:
			ExecutorException(const char* const message) noexcept;

			~ExecutorException() = default;
		};
	}
}