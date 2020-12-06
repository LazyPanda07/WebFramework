#include "MissingLoadTypeException.h"

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		MissingLoadTypeException::MissingLoadTypeException() :
			BaseExecutorException(::exceptions::missingLoadType)
		{

		}
	}
}