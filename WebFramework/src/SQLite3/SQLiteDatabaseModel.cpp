#include "SQLiteDatabaseModel.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

namespace framework
{
	namespace sqlite
	{
		bool SQLiteDatabaseModel::isNumber(const string& source)
		{
			if (source.empty())
			{
				return false;
			}

			static constexpr string_view symbols = "0123456789-.";

			if (all_of(source.begin(), source.end(), [](const char& c) { return find(symbols.begin(), symbols.end(), c) != symbols.end(); }))
			{
				size_t check = 0;

				for (const auto& i : source)
				{
					if (i == '.')
					{
						check++;
					}
				}

				if (check > 1)
				{
					return false;
				}

				return true;
			}

			return false;
		}

		string SQLiteDatabaseModel::executeQuery(const string& query)
		{
			sqlite3_stmt* result = nullptr;
			string output;
			int code;

			sqlite3_prepare_v2(*db, query.data(), -1, &result, nullptr);

			while ((code = sqlite3_step(result)) == SQLITE_ROW)
			{
				for (size_t i = 0; i < sqlite3_column_count(result); i++)
				{
					output.append(reinterpret_cast<const char*>(sqlite3_column_text(result, i))).append(" ");
				}

				output.pop_back();

				output += '\n';
			}

			if (code != SQLITE_DONE)
			{
				throw runtime_error(sqlite3_errmsg(*db));
			}

			sqlite3_finalize(result);

			return output;
		}

		SQLiteDatabaseModel::SQLiteDatabaseModel(const string& tableName, SQLiteDatabase&& db) :
			tableName(tableName),
			db(move(db))
		{

		}

		SQLiteDatabaseModel::SQLiteDatabaseModel(SQLiteDatabaseModel&& other) noexcept :
			tableName(move(other.tableName)),
			db(move(db))
		{

		}

		SQLiteDatabaseModel& SQLiteDatabaseModel::operator = (SQLiteDatabaseModel&& other) noexcept
		{
			tableName = move(other.tableName);
			db = move(other.db);

			return *this;
		}

		string SQLiteDatabaseModel::rawQuery(const string& query, SQLiteDatabaseModel::queryType type)
		{
			if (type == queryType::read)
			{
				shared_lock<shared_mutex> lock(readWriteMutex);

				return this->executeQuery(query);
			}
			else if (type == queryType::write)
			{
				unique_lock<shared_mutex> lock(readWriteMutex);

				return this->executeQuery(query);
			}

			throw invalid_argument("invalid type variable");
		}

		void SQLiteDatabaseModel::createTableQuery(const vector<pair<string, string>>& attributes)
		{
			string fields = "(";

			for (const auto& [key, value] : attributes)
			{
				fields += key + ' ' + value + ", ";
			}

			fields.replace(fields.end() - 2, fields.end(), ")");

			this->rawQuery("CREATE TABLE IF NOT EXISTS " + tableName + ' ' + fields, queryType::write);
		}

		void SQLiteDatabaseModel::dropTableQuery()
		{
			this->rawQuery("DROP TABLE IF EXISTS " + tableName, queryType::write);
		}

		void SQLiteDatabaseModel::recreateTableQuery(const vector<pair<string, string>>& attributes)
		{
			this->dropTableQuery();

			this->createTableQuery(attributes);
		}

		void SQLiteDatabaseModel::insertQuery(const unordered_map<string,string>& attributes)
		{
			string keys;
			string values;

			for (const auto& [key, value] : attributes)
			{
				keys += key + ", ";

				if (isNumber(value))
				{
					values += value + ", ";
				}
				else
				{
					values += '\'' + value + "', ";
				}
			}

			keys = '(' + string(keys.begin(), keys.end() - 2) + ')';
			values = '(' + string(values.begin(), values.end() - 2) + ')';

			this->rawQuery("INSERT INTO " + tableName + " " + keys + " VALUES " + values, queryType::write);
		}

		void SQLiteDatabaseModel::updateQuery(const unordered_map<string, string>& attributes, const string& fieldName, const string& fieldValue)
		{
			string query = "UPDATE " + tableName + " SET ";

			for (const auto& [key, value] : attributes)
			{
				query += key + " = " + (isNumber(value) ? value : '\'' + value + '\'') + ' ';
			}

			query += "WHERE " + fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'');

			this->rawQuery(query, queryType::write);
		}

		void SQLiteDatabaseModel::deleteQuery(const string& fieldName, const string& fieldValue)
		{
			string query = "DELETE FROM " + tableName + " WHERE " + fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'');

			this->rawQuery(query, queryType::write);
		}

		string SQLiteDatabaseModel::selectAllQuery()
		{
			return this->rawQuery("SELECT * FROM " + tableName, queryType::read);
		}

		string SQLiteDatabaseModel::selectByFieldQuery(const string& fieldName, const string& fieldValue)
		{
			return this->rawQuery("SELECT * FROM " + tableName + " WHERE " + fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\''), queryType::read);
		}

		string SQLiteDatabaseModel::selectByFieldQuery(const unordered_map<string, string>& attributes)
		{
			string query = "SELECT * FROM " + tableName + " WHERE ";

			for (const auto& [fieldName, fieldValue] : attributes)
			{
				query += fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue) + ", ";
			}

			query.pop_back();
			query.pop_back();

			return this->rawQuery(query, queryType::read);
		}

		const string& SQLiteDatabaseModel::getTableName() const
		{
			return tableName;
		}
	}
}
