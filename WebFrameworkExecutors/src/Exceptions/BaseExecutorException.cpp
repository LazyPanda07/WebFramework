#include "BaseExecutorException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		BaseExecutorException::BaseExecutorException(string_view message) :
			BaseWebFrameworkException(message)
		{

		}
	}
}