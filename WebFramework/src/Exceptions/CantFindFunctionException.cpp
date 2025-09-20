#include "CantFindFunctionException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	CantFindFunctionException::CantFindFunctionException(std::string_view createFunctionName) :
		BaseExecutorException(std::format("{} {} function. {}", ::exceptions::cantFindFunction, createFunctionName, ::exceptions::useMacroToMakeCreationFunction))
	{

	}
}
