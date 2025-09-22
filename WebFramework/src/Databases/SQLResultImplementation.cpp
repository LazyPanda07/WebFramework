#include "SQLResultImplementation.h"

#include "SQLValueImplementation.h"

namespace framework
{
	SQLResultImplementation::SQLResultImplementation(database::SQLResult&& result) :
		result(std::move(result))
	{

	}

	size_t SQLResultImplementation::size() const
	{
		return result.size();
	}

	void SQLResultImplementation::iterate(void(initBuffer)(size_t size, void* buffer), void (*callback)(const char** columnName, const void** columnValue, size_t size, size_t index, void* buffer), void* buffer)
	{
		initBuffer(result.size(), buffer);

		std::vector<const char*> columnNames;
		std::vector<SQLValueImplementation> columnValues;
		std::vector<const void*> pointers;

		if (result.size())
		{
			columnNames.resize(result[0].size());
			columnValues.resize(result[0].size());
			pointers.resize(result[0].size());
		}

		for (size_t i = 0, index = 0; i < result.size(); i++, index = 0)
		{
			for (const auto& [key, value] : result[i])
			{
				columnNames[index] = key.data();
				columnValues[index++] = SQLValueImplementation(value);
			}

			for (size_t j = 0; j < pointers.size(); j++)
			{
				pointers[j] = &columnValues[j];
			}

			callback(columnNames.data(), pointers.data(), pointers.size(), i, buffer);
		}
	}
}
