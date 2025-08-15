#include "SQLResultExecutors.h"

using namespace std;

namespace framework
{
	void SQLResultExecutors::reserveSize(size_t size, void* buffer)
	{
		static_cast<ValueType*>(buffer)->resize(size);
	}

	void SQLResultExecutors::fill(const char* columnName, const void* value, size_t index, size_t size, void* buffer)
	{
		(*static_cast<ValueType*>(buffer))[index].try_emplace(columnName, SQLValueExecutors(static_cast<const interfaces::ISQLValue*>(value)));
	}

	SQLResultExecutors::SQLResultExecutors(interfaces::ISQLResult* implementation)
	{
		implementation->iterate(&SQLResultExecutors::reserveSize, &SQLResultExecutors::fill, &rows);
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
