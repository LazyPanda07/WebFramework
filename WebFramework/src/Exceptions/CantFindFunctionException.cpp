#include "Exceptions/CantFindFunctionException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	CantFindFunctionException::CantFindFunctionException(std::string_view createFunctionName) :
		ExecutorException(std::format("{} {} function. {}", ::exceptions::cantFindFunction, createFunctionName, ::exceptions::useMacroToMakeCreationFunction))
	{

	}
}
