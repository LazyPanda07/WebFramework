#include "Exceptions/MissingLoadTypeException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	MissingLoadTypeException::MissingLoadTypeException(std::string_view className) :
		ExecutorException(std::format("{} {}", ::exceptions::missingLoadType, className))
	{

	}
}
