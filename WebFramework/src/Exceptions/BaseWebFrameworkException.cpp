#include "BaseWebFrameworkException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		BaseWebFrameworkException::BaseWebFrameworkException(const string& errorMessage) :
			runtime_error(errorMessage)
		{

		}
	}
}
