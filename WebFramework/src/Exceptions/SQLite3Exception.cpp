#include "SQLite3Exception.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		SQLite3Exception::SQLite3Exception(const string& errorMessage) :
			DatabaseException(errorMessage)
		{

		}
	}
}
