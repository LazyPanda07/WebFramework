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

			static constexpr string_view symbols = "0123456789-.,";

			if (all_of(source.begin(), source.end(), [](const char& c) { return find(symbols.begin(), symbols.end(), c) != symbols.end(); }))
			{
				size_t check = 0;

				for (const auto& i : source)
				{
					if (i == '.' || i == ',')
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

		utility::SQLiteResult SQLiteDatabaseModel::executeQuery(const string& query)
		{
			sqlite3_stmt* result = nullptr;
			vector<unordered_map<string, string>> output;
			int code;

			sqlite3_prepare_v2(*db, query.data(), -1, &result, nullptr);

			while ((code = sqlite3_step(result)) == SQLITE_ROW)
			{
				for (size_t i = 0; i < sqlite3_column_count(result); i++)
				{
					output.back().insert
					(
						make_pair
						(
							sqlite3_column_name(result, i),
							reinterpret_cast<const char*>(sqlite3_column_text(result, i))
						)
					);
				}
			}

			if (code != SQLITE_DONE)
			{
				throw runtime_error(sqlite3_errmsg(*db));
			}

			sqlite3_finalize(result);

			if (query.find("INSERT") != string::npos)
			{
				try
				{
					return this->executeQuery("SELECT * FROM " + this->getTableName() + " WHERE id=" + to_string(sqlite3_last_insert_rowid(db.db)));
				}
				catch (const runtime_error&)
				{

				}
			}

			return utility::SQLiteResult(move(output));
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

		utility::SQLiteResult SQLiteDatabaseModel::rawQuery(const string& query, SQLiteDatabaseModel::queryType type)
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

		utility::SQLiteResult SQLiteDatabaseModel::insertQuery(const map<string,string>& attributes)
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

			return this->rawQuery("INSERT INTO " + tableName + " " + keys + " VALUES " + values, queryType::write);
		}

		void SQLiteDatabaseModel::updateQuery(const map<string, string>& attributes, const string& fieldName, const string& fieldValue)
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

		void SQLiteDatabaseModel::deleteQuery(const map<string, string>& attributes)
		{
			string query = "DELETE FROM " + tableName + " WHERE ";

			for (const auto& [fieldName, fieldValue] : attributes)
			{
				query += fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'') + " AND ";
			}

			query.resize(query.size() - 5);

			this->rawQuery(query, queryType::write);
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectAllQuery()
		{
			return this->rawQuery("SELECT * FROM " + tableName, queryType::read);
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectByFieldQuery(const string& fieldName, const string& fieldValue)
		{
			return this->rawQuery("SELECT * FROM " + tableName + " WHERE " + fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\''), queryType::read);
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectByFieldQuery(const map<string, string>& attributes)
		{
			string query = "SELECT * FROM " + tableName + " WHERE ";

			for (const auto& [fieldName, fieldValue] : attributes)
			{
				query += fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'') + " AND ";
			}

			query.resize(query.size() - 5);

			return this->rawQuery(query, queryType::read);
		}

		const string& SQLiteDatabaseModel::getTableName() const
		{
			return tableName;
		}
	}
}
