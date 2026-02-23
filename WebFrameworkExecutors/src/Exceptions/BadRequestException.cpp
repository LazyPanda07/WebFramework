#include "BadRequestException.h"

namespace framework::exceptions
{
	BadRequestException::BadRequestException(std::string_view additionalInformation) :
		ExecutorException(std::format("Bad request from client. {}", additionalInformation))
	{

	}
}
