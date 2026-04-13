#include "Exceptions/AlreadyLoggedException.h"

namespace framework::exceptions
{
	AlreadyLoggedException::AlreadyLoggedException(std::string&& message) :
		message(std::move(message))
	{

	}

	const char* AlreadyLoggedException::what() const
	{
		return message.data();
	}
}
