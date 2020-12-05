#include "CantFindFunctionException.h"

#include "WebFrameworkConstants.h"

namespace framework
{
	namespace exceptions
	{
		CantFindFunctionException::CantFindFunctionException() :
			BaseExecutorException(::exceptions::cantFindFunction)
		{

		}
	}
}
