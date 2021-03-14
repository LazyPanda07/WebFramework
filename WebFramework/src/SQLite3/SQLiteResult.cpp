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
