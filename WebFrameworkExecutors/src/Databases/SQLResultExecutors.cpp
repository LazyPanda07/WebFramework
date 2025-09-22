#include "SQLResultExecutors.h"

namespace framework
{
	void SQLResultExecutors::reserveSize(size_t size, void* buffer)
	{
		static_cast<ValueType*>(buffer)->resize(size);
	}

	void SQLResultExecutors::fill(const char** columnNames, const void** values, size_t size, size_t index, void* buffer)
	{
		for (size_t i = 0; i < size; i++)
		{
			(*static_cast<ValueType*>(buffer))[index].try_emplace(columnNames[i], SQLValueExecutors(static_cast<const interfaces::ISQLValue*>(values[i])));
		}
	}

	SQLResultExecutors::SQLResultExecutors(interfaces::ISQLResult* implementation)
	{
		implementation->iterate(&SQLResultExecutors::reserveSize, &SQLResultExecutors::fill, &rows);
	}

	std::unordered_map<std::string, SQLValueExecutors>& SQLResultExecutors::at(size_t index)
	{
		return rows.at(index);
	}

	const std::unordered_map<std::string, SQLValueExecutors>& SQLResultExecutors::at(size_t index) const
	{
		return rows.at(index);
	}

	size_t SQLResultExecutors::size() const
	{
		return rows.size();
	}

	SQLResultExecutors::ValueType::iterator SQLResultExecutors::begin()
	{
		return rows.begin();
	}

	SQLResultExecutors::ValueType::iterator SQLResultExecutors::end()
	{
		return rows.end();
	}

	SQLResultExecutors::operator ValueType& ()
	{
		return rows;
	}

	SQLResultExecutors::operator const ValueType& () const
	{
		return rows;
	}

	std::unordered_map<std::string, SQLValueExecutors>& SQLResultExecutors::operator[](size_t index)
	{
		return rows[index];
	}

	const std::unordered_map<std::string, SQLValueExecutors>& SQLResultExecutors::operator[](size_t index) const
	{
		return rows[index];
	}
}
