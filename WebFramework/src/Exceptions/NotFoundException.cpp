#include "NotFoundException.h"

#include "Framework/WebFrameworkConstants.h"

using namespace std;

namespace framework::exceptions
{
	NotFoundException::NotFoundException(string_view additionalInformation) :
		BaseExecutorException(format("{}. {}", ::exceptions::notFound, additionalInformation))
	{

	}
}
