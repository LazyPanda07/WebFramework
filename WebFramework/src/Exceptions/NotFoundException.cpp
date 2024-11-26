#include "NotFoundException.h"

namespace framework
{
	namespace exceptions
	{
		NotFoundException::NotFoundException(std::string_view additionalInformation) :
			BaseExecutorException(format("{}. {}", ::exceptions::notFound, additionalInformation))
		{

		}
	}
}
