#include "BaseWebFrameworkException.h"

namespace framework
{
	namespace exceptions
	{
		BaseWebFrameworkException::BaseWebFrameworkException(std::string_view errorMessage) :
			runtime_error(std::string(errorMessage.data(), errorMessage.size()))
		{

		}
	}
}
