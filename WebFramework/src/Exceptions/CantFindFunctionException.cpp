#include "CantFindFunctionException.h"

#include "Framework/WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		CantFindFunctionException::CantFindFunctionException(string_view createFunctionName) :
			BaseExecutorException(format("{} {} function. {}", ::exceptions::cantFindFunction, createFunctionName, ::exceptions::useMacroToMakeCreationFunction))
		{

		}
	}
}
