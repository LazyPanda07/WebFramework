#include "Exceptions/NotFoundException.h"

namespace framework::exceptions
{
	NotFoundException::NotFoundException(std::string_view additionalInformation) :
		ExecutorException(std::format("Not found. {}", additionalInformation))
	{

	}
}
