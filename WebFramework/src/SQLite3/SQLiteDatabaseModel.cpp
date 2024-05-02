#include "SQLiteDatabaseModel.h"

#include "Exceptions/SQLite3Exception.h"

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

			if (all_of(source.begin(), source.end(), [](char c) { return find(symbols.begin(), symbols.end(), c) != symbols.end(); }))
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

		utility::SQLiteResult SQLiteDatabaseModel::execute(const string& query)
		{
			sqlite3_stmt* result = nullptr;
			vector<unordered_map<string, string>> output;
			int code;

			sqlite3_prepare_v2(**db, query.data(), -1, &result, nullptr);

			while ((code = sqlite3_step(result)) == SQLITE_ROW)
			{
				for (int i = 0; i < sqlite3_column_count(result); i++)
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
				throw exceptions::SQLite3Exception(sqlite3_errmsg(**db));
			}

			sqlite3_finalize(result);

			if (query.find("INSERT") != string::npos)
			{
				try
				{
					return this->execute("SELECT * FROM " + this->getTableName() + " WHERE id=" + to_string(sqlite3_last_insert_rowid(**db)));
				}
				catch (const exceptions::SQLite3Exception&)
				{

				}
			}

			return utility::SQLiteResult(move(output));
		}

		SQLiteDatabaseModel::SQLiteDatabaseModel(const string& tableName) :
			tableName(tableName)
		{

		}

		utility::SQLiteResult SQLiteDatabaseModel::raw(const string& query)
		{
			return this->execute(query);
		}

		void SQLiteDatabaseModel::createTable(const vector<pair<string, string>>& attributes)
		{
			string fields = "(";

			for (const auto& [key, value] : attributes)
			{
				fields += key + ' ' + value + ", ";
			}

			fields.replace(fields.end() - 2, fields.end(), ")");

			this->raw("CREATE TABLE IF NOT EXISTS " + tableName + ' ' + fields);
		}

		void SQLiteDatabaseModel::dropTable()
		{
			this->raw("DROP TABLE IF EXISTS " + tableName);
		}

		void SQLiteDatabaseModel::recreateTable(const vector<pair<string, string>>& attributes)
		{
			this->dropTable();

			this->createTable(attributes);
		}

		utility::SQLiteResult SQLiteDatabaseModel::insert(const unordered_map<string,string>& attributes)
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

			return this->raw("INSERT INTO " + tableName + " " + keys + " VALUES " + values);
		}

		void SQLiteDatabaseModel::update(const unordered_map<string, string>& attributes, const string& fieldName, const string& fieldValue)
		{
			string query = "UPDATE " + tableName + " SET ";

			for (const auto& [key, value] : attributes)
			{
				query += key + " = " + (isNumber(value) ? value : '\'' + value + '\'') + ' ';
			}

			query += "WHERE " + fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'');

			this->raw(query);
		}

		void SQLiteDatabaseModel::deleteQuery(const string& fieldName, const string& fieldValue)
		{
			string query = "DELETE FROM " + tableName + " WHERE " + fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'');

			this->raw(query);
		}

		void SQLiteDatabaseModel::deleteQuery(const unordered_map<string, string>& attributes)
		{
			string query = "DELETE FROM " + tableName + " WHERE ";

			for (const auto& [fieldName, fieldValue] : attributes)
			{
				query += fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'') + " AND ";
			}

			query.resize(query.size() - 5);

			this->raw(query);
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectAll()
		{
			return this->raw("SELECT * FROM " + tableName);
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectByField(const string& fieldName, const string& fieldValue)
		{
			return this->raw("SELECT * FROM " + tableName + " WHERE " + fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\''));
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectByField(const unordered_map<string, string>& attributes)
		{
			string query = "SELECT * FROM " + tableName + " WHERE ";

			for (const auto& [fieldName, fieldValue] : attributes)
			{
				query += fieldName + " = " + (isNumber(fieldValue) ? fieldValue : '\'' + fieldValue + '\'') + " AND ";
			}

			query.resize(query.size() - 5);

			return this->raw(query);
		}

		const string& SQLiteDatabaseModel::getTableName() const
		{
			return tableName;
		}
	}
}
