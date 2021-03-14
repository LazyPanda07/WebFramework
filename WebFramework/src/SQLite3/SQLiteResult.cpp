#include "SQLiteResult.h"

using namespace std;

namespace framework
{
	namespace sqlite
	{
		namespace utility
		{
			SQLiteResult::SQLiteResult(size_t rowCount) :
				rows(rowCount)
			{

			}

			SQLiteResult::SQLiteResult(const SQLiteResult& other) :
				rows(other.rows)
			{

			}

			SQLiteResult::SQLiteResult(SQLiteResult&& other) noexcept :
				rows(move(other.rows))
			{

			}

			SQLiteResult& SQLiteResult::operator = (const SQLiteResult& other)
			{
				rows = other.rows;

				return *this;
			}

			SQLiteResult& SQLiteResult::operator = (SQLiteResult&& other) noexcept
			{
				rows = move(other.rows);

				return *this;
			}

			void SQLiteResult::addRow()
			{
				rows.emplace_back();
			}

			void SQLiteResult::resize(size_t rowCount)
			{
				rows.resize(rowCount);
			}

			size_t SQLiteResult::size() const
			{
				return rows.size();
			}

			unordered_map<string, string>& SQLiteResult::operator [] (size_t index)
			{
				return rows[index];
			}

			const unordered_map<string, string>& SQLiteResult::operator [] (size_t index) const
			{
				return rows[index];
			}

			unordered_map<string, string>& SQLiteResult::front()
			{
				return rows.front();
			}

			const unordered_map<string, string>& SQLiteResult::front() const
			{
				return rows.front();
			}

			unordered_map<string, string>& SQLiteResult::back()
			{
				return rows.back();
			}

			const unordered_map<string, string>& SQLiteResult::back() const
			{
				return rows.back();
			}

			SQLiteResult::const_iterator SQLiteResult::begin() const noexcept
			{
				return rows.begin();
			}

			SQLiteResult::const_iterator SQLiteResult::end() const noexcept
			{
				return rows.end();
			}
		}
	}
}
