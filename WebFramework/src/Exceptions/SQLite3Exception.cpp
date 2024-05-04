#include "SQLite3Exception.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		SQLite3Exception::SQLite3Exception(const string& errorMessage, string_view query) :
			DatabaseException(format("Exception: {}. Query: {}", errorMessage, query))
		{

		}
	}
}
