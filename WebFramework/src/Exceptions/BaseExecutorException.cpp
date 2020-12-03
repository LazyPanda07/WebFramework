#include "BaseExecutorException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		BaseExecutorException::BaseExecutorException(const string& message) :
			runtime_error(message)
		{

		}
	}
}