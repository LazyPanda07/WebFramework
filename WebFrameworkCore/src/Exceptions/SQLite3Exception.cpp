#include "SQLite3Exception.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		SQLite3Exception::SQLite3Exception(string_view errorMessage, string_view query) :
			DatabaseException(format("{}. Query: {}", errorMessage, query))
		{

		}
	}
}
