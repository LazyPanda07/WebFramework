#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		class FileDoesNotExistException : public std::runtime_error
		{
		public:
			FileDoesNotExistException();

			~FileDoesNotExistException() = default;
		};
	}
}