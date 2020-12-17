#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		class CantLoadSourceException : public std::runtime_error
		{
		public:
			CantLoadSourceException(const std::string& source);

			~CantLoadSourceException() = default;
		};
	}
}
