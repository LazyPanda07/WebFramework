#include "DatabaseException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		DatabaseException::DatabaseException(const string& errorMessage) :
			BaseWebFrameworkException(errorMessage)
		{

		}
	}
}
