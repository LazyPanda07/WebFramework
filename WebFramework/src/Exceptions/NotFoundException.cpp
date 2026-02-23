#include "NotFoundException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	NotFoundException::NotFoundException(std::string_view additionalInformation) :
		ExecutorException(std::format("{}. {}", ::exceptions::notFound, additionalInformation))
	{

	}
}
