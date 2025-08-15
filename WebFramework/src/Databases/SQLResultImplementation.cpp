#include "SQLResultImplementation.h"

#include "SQLValueImplementation.h"

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

	void SQLResultImplementation::iterate(void(initBuffer)(size_t size, void* buffer), void (*callback)(const char* columnName, const void* columnValue, size_t index, size_t size, void* buffer), void* buffer)
	{
		initBuffer(result.size(), buffer);

		for (size_t i = 0; i < result.size(); i++)
		{
			for (const auto& [key, value] : result[i])
			{
				SQLValueImplementation temp(value);

				callback(key.data(), &temp, i, result.size(), buffer);
			}
		}
	}
}
