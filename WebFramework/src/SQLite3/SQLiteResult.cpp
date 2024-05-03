#include "SQLiteResult.h"

using namespace std;

namespace framework
{
	namespace sqlite
	{
		namespace utility
		{
			SQLiteResult::SQLiteResult(vector<unordered_map<string, string>>&& rows) noexcept :
				rows(move(rows))
			{

			}

			size_t SQLiteResult::size() const
			{
				return rows.size();
			}

			bool SQLiteResult::isEmpty() const
			{
				return rows.empty();
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

			unordered_map<string, string>& SQLiteResult::at(size_t index)
			{
				return rows.at(index);
			}

			const unordered_map<string, string>& SQLiteResult::at(size_t index) const
			{
				return rows.at(index);
			}

			string& SQLiteResult::at(size_t index, const string& columnName)
			{
				return rows[index].at(columnName);
			}

			const string& SQLiteResult::at(size_t index, const string& columnName) const
			{
				return rows[index].at(columnName);
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
