#include "SQLResultImplementation.h"

using namespace std;

namespace framework
{
	SQLResultImplementation::SQLResultImplementation(database::SQLResult&& result) :
		result(move(result))
	{

	}

	size_t SQLResultImplementation::size() const
	{
		return result.size();
	}

	void SQLResultImplementation::iterate(const char* columnName, interfaces::ISQLValue* columnValue, size_t index, size_t size)
	{

	}
}
