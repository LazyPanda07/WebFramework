#include "SQLiteDatabaseModel.h"

#include "Exceptions/SQLite3Exception.h"

using namespace std;

namespace framework
{
	namespace sqlite
	{
		bool SQLiteDatabaseModel::isNumber(string_view source)
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

		string SQLiteDatabaseModel::convertToValue(string_view source)
		{
			return SQLiteDatabaseModel::isNumber(source) ? string(source) : format("'{}'", source);
		}

		utility::SQLiteResult SQLiteDatabaseModel::execute(const string& query)
		{
			sqlite3_stmt* result = nullptr;
			vector<unordered_map<string, string>> output;
			int code;

			sqlite3_prepare_v2(**database, query.data(), -1, &result, nullptr);

			while ((code = sqlite3_step(result)) == SQLITE_ROW)
			{
				unordered_map<string, string>& row = output.emplace_back();
				int columnCount = sqlite3_column_count(result);

				for (int i = 0; i < columnCount; i++)
				{
					row.emplace
					(
						sqlite3_column_name(result, i),
						reinterpret_cast<const char*>(sqlite3_column_text(result, i))
					);
				}
			}

			if (code != SQLITE_DONE)
			{
				throw exceptions::SQLite3Exception(sqlite3_errmsg(**database), query);
			}

			sqlite3_finalize(result);

			if (query.find("INSERT") != string::npos)
			{
				return this->execute(format("SELECT * FROM {} WHERE id = {}", this->getTableName(), to_string(sqlite3_last_insert_rowid(**database))));
			}

			return utility::SQLiteResult(move(output));
		}

		SQLiteDatabaseModel::SQLiteDatabaseModel() :
			database(databaseConstructor) //-V670
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
				fields += format("{} {}, ", key, value);
			}

			fields.replace(fields.end() - 2, fields.end(), ")");

			this->raw(format("CREATE TABLE IF NOT EXISTS {} {}", this->getTableName(), fields));
		}

		void SQLiteDatabaseModel::dropTable()
		{
			this->raw(format("DROP TABLE IF EXISTS {}", this->getTableName()));
		}

		void SQLiteDatabaseModel::recreateTable(const vector<pair<string, string>>& attributes)
		{
			this->dropTable();

			this->createTable(attributes);
		}

		utility::SQLiteResult SQLiteDatabaseModel::insert(const unordered_map<string, string>& attributes)
		{
			string keys;
			string values;

			for (const auto& [key, value] : attributes)
			{
				keys += format("{}, ", key);
				values += format("{}, ", SQLiteDatabaseModel::convertToValue(value));
			}

			return this->raw(format("INSERT INTO {} ({}) VALUES ({})", this->getTableName(), string(keys.begin(), keys.end() - 2), string(values.begin(), values.end() - 2)));
		}

		void SQLiteDatabaseModel::update(const unordered_map<string, string>& attributes, const string& fieldName, const string& fieldValue)
		{
			string query = format("UPDATE {} SET ", this->getTableName());

			for (const auto& [key, value] : attributes)
			{
				query += format("{} = {} ", key, SQLiteDatabaseModel::convertToValue(value));
			}

			query += format("WHERE {} = {}", fieldName, SQLiteDatabaseModel::convertToValue(fieldValue));

			this->raw(query);
		}

		void SQLiteDatabaseModel::deleteQuery(const string& fieldName, const string& fieldValue)
		{
			this->raw(format("DELETE FROM {} WHERE {} = {}", this->getTableName(), fieldName, SQLiteDatabaseModel::convertToValue(fieldValue)));
		}

		void SQLiteDatabaseModel::deleteQuery(const unordered_map<string, string>& attributes)
		{
			string query = format("DELETE FROM {} WHERE ", this->getTableName());

			for (const auto& [fieldName, fieldValue] : attributes)
			{
				query += format("{} = {} AND ", fieldName, SQLiteDatabaseModel::convertToValue(fieldValue));
			}

			query.resize(query.size() - 5);

			this->raw(query);
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectAll()
		{
			return this->raw(format("SELECT * FROM {}", this->getTableName()));
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectByField(const string& fieldName, const string& fieldValue)
		{
			return this->raw(format("SELECT * FROM {} WHERE {} = {}", this->getTableName(), fieldName, SQLiteDatabaseModel::convertToValue(fieldValue)));
		}

		utility::SQLiteResult SQLiteDatabaseModel::selectByField(const unordered_map<string, string>& attributes)
		{
			string query = format("SELECT * FROM {} WHERE ", this->getTableName());

			for (const auto& [fieldName, fieldValue] : attributes)
			{
				query += format("{} = {} AND ", fieldName, SQLiteDatabaseModel::convertToValue(fieldValue));
			}

			query.resize(query.size() - 5);

			return this->raw(query);
		}
	}
}
