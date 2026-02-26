#include "Exceptions/ExecutorException.h"

namespace framework::exceptions
{
	ExecutorException::ExecutorException(std::string_view message) :
		BaseWebFrameworkException(message)
	{

	}
}
