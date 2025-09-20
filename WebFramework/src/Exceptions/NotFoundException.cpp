#include "NotFoundException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	NotFoundException::NotFoundException(std::string_view additionalInformation) :
		BaseExecutorException(std::format("{}. {}", ::exceptions::notFound, additionalInformation))
	{

	}
}
