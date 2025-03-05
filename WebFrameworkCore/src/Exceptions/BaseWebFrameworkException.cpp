#include "BaseWebFrameworkException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		BaseWebFrameworkException::BaseWebFrameworkException(string_view errorMessage) :
			runtime_error(string(errorMessage.data(), errorMessage.size()))
		{

		}
	}
}
