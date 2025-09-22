#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		BaseExecutorException::BaseExecutorException(std::string_view message) :
			BaseWebFrameworkException(message)
		{

		}
	}
}