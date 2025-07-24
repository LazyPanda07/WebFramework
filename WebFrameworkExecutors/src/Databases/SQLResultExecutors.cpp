#include "SQLResultExecutors.h"

using namespace std;

namespace framework
{
	void SQLResultExecutors::fill(const char* columnName, const interfaces::ISQLValue* value, size_t index, size_t size, void* data)
	{
		ValueType& rows = *static_cast<ValueType*>(data);

		if (rows.empty())
		{
			rows.resize(size);
		}

		rows[index].try_emplace(columnName, SQLValueExecutors(value));
	}

	SQLResultExecutors::SQLResultExecutors(interfaces::ISQLResult* implementation)
	{
		implementation->iterate(&SQLResultExecutors::fill, &rows);
	}

	unordered_map<string, SQLValueExecutors>& SQLResultExecutors::at(size_t index)
	{
		return rows.at(index);
	}

	const unordered_map<string, SQLValueExecutors>& SQLResultExecutors::at(size_t index) const
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

	unordered_map<string, SQLValueExecutors>& SQLResultExecutors::operator[](size_t index)
	{
		return rows[index];
	}

	const unordered_map<string, SQLValueExecutors>& SQLResultExecutors::operator[](size_t index) const
	{
		return rows[index];
	}
}
