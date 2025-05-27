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

	void SQLResultImplementation::iterate(void (*callback)(const char* columnName, const interfaces::ISQLValue* columnValue, size_t index, size_t size))
	{
		for (size_t i = 0; i < result.size(); i++)
		{
			for (const auto& [key, value] : result[i])
			{
				SQLValueImplementation valueImplementation(value);

				callback(key.data(), &value, i, result.size());
			}
		}
	}
}
