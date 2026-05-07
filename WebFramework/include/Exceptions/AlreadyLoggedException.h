#pragma once

#include <exception>
#include <string>

namespace framework::exceptions
{
	class AlreadyLoggedException : public std::exception
	{
	private:
		std::string message;

	public:
		AlreadyLoggedException(std::string&& message);

		const char* what() const noexcept override;

		~AlreadyLoggedException() = default;
	};
}
