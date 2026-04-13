#pragma once

#include <exception>
#include <string>

namespace framework::exceptions
{
	class AlreadyLoggedException : public std::exception
	{
	public:
		AlreadyLoggedException(std::string&& message);

		~AlreadyLoggedException() = default;
	};
}
