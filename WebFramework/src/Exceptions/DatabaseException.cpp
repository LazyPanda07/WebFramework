#include "DatabaseException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		DatabaseException::DatabaseException(string_view errorMessage) :
			BaseWebFrameworkException(errorMessage)
		{

		}
	}
}
