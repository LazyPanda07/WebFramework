#include "Exceptions/AlreadyLoggedException.h"

namespace framework::exceptions
{
	AlreadyLoggedException::AlreadyLoggedException(std::string&& message) :
		exception(message.data())
	{

	}
}
