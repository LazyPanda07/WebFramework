#include "MissingLoadTypeException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		MissingLoadTypeException::MissingLoadTypeException(string_view className) :
			BaseExecutorException(format("{}{}", ::exceptions::missingLoadType, className))
		{

		}
	}
}
