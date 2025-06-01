#include "SQLResult.h"

using namespace std;

namespace framework
{
	void SQLResult::fill(const char* columnName, const interfaces::ISQLValue* value, size_t index, size_t size, void* data)
	{
		ValueType& rows = *static_cast<ValueType*>(data);

		if (rows.empty())
		{
			rows.resize(size);
		}

		rows[index].try_emplace(columnName, SQLValue(value));
	}

	SQLResult::SQLResult(interfaces::ISQLResult* implementation)
	{
		implementation->iterate(&SQLResult::fill, &rows);
	}

	unordered_map<string, SQLValue>& SQLResult::at(size_t index)
	{
		return rows.at(index);
	}

	const unordered_map<string, SQLValue>& SQLResult::at(size_t index) const
	{
		return rows.at(index);
	}

	size_t SQLResult::size() const
	{
		return rows.size();
	}

	SQLResult::ValueType::iterator SQLResult::begin()
	{
		return rows.begin();
	}

	SQLResult::ValueType::iterator SQLResult::end()
	{
		return rows.end();
	}

	SQLResult::operator ValueType& ()
	{
		return rows;
	}

	SQLResult::operator const ValueType& () const
	{
		return rows;
	}

	unordered_map<string, SQLValue>& SQLResult::operator[](size_t index)
	{
		return rows[index];
	}

	const unordered_map<string, SQLValue>& SQLResult::operator[](size_t index) const
	{
		return rows[index];
	}
}
