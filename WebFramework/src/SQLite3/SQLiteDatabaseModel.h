#pragma once

#include <string>
#include <unordered_map>

#include "SQLiteDatabase.h"

namespace framework
{
	namespace sqlite
	{
		class SQLiteDatabaseModel
		{
		private:
			const std::string tableName;
			SQLiteDatabase db;

		private:
			static bool isNumber(const std::string& source);

		public:
			SQLiteDatabaseModel(const std::string& tableName, SQLiteDatabase&& db);

			SQLiteDatabaseModel(const SQLiteDatabaseModel&) = delete;

			SQLiteDatabaseModel& operator = (const SQLiteDatabaseModel&) = delete;

			SQLiteDatabaseModel(SQLiteDatabaseModel&&) noexcept = delete;

			SQLiteDatabaseModel& operator = (SQLiteDatabaseModel&&) noexcept = delete;

			std::string rawQuery(const std::string& query);

			void createTableQuery(const std::unordered_map<std::string, std::string>& attributes);

			void dropTableQuery();

			void recreateTableQuery(const std::unordered_map<std::string, std::string>& attributes);

			void insertQuery(const std::unordered_map<std::string, std::string>& attributes);

			void updateQuery(const std::unordered_map<std::string, std::string>& attributes, const std::string& fieldName, const std::string& fieldValue);

			void deleteQuery(const std::string& fieldName, const std::string& fieldValue);

			std::string selectAllQuery();

			std::string selectByFieldQuery(const std::string& fieldName, const std::string& fieldValue);

			~SQLiteDatabaseModel() = default;
		};
	}
}
