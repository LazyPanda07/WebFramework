#include "Exceptions/ForbiddenException.h"

namespace framework::exceptions
{
	ForbiddenException::ForbiddenException(std::string_view additionalInformation) :
		ExecutorException(std::format("Forbidden response to client request. {}", additionalInformation))
	{

	}
}
