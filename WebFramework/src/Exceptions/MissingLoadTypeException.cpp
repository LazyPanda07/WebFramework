#include "MissingLoadTypeException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	MissingLoadTypeException::MissingLoadTypeException(std::string_view className) :
		BaseExecutorException(std::format("{} {}", ::exceptions::missingLoadType, className))
	{

	}
}
