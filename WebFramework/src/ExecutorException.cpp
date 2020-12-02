#include "ExecutorException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		ExecutorException::ExecutorException(const char* const message) noexcept :
			exception(message)
		{

		}
	}
}