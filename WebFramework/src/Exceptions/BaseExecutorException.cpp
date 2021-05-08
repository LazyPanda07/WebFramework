#include "BaseExecutorException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		BaseExecutorException::BaseExecutorException(const string& message) :
			BaseWebFrameworkException(message)
		{

		}
	}
}