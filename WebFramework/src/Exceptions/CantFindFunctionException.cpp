#include "CantFindFunctionException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		CantFindFunctionException::CantFindFunctionException(const string& createFunctionName) :
			BaseExecutorException(::exceptions::cantFindFunction + createFunctionName + " function" + ". " + ::exceptions::useMacroToMakeCreationFunction)
		{

		}
	}
}
