#pragma once

#include "headers.h"

#include "SQLiteDatabase.h"
#include "SQLiteResult.h"

#define DECLARE_DATABASE_NAME(DatabaseName) public: static constexpr std::string_view databaseName = DatabaseName
#define DECLARE_TABLE_NAME(TableName) public: static constexpr std::string_view tableName = TableName
#define DECLARE_DATABASE_AND_TABLE_NAME(DatabaseName, TableName) DECLARE_DATABASE_NAME(DatabaseName); \
	DECLARE_TABLE_NAME(TableName);

namespace framework
{
	namespace sqlite
	{
		/// <summary>
		/// Providing SELECT, INSERT, UPDATE, DELETE or raw queries for SQLiteDatabase
		/// </summary>
		class WEB_FRAMEWORK_API SQLiteDatabaseModel
		{
		public:
			static constexpr std::string_view databaseName = "";
			static constexpr std::string_view tableName = "";

		protected:
			std::shared_ptr<SQLiteDatabase> database;

		protected:
			/// <summary>
			/// Check that string represent number
			/// </summary>
			/// <param name="source">attribute value</param>
			/// <returns>true if source is correct number, false otherwise</returns>
			static bool isNumber(const std::string& source);

		protected:
			/// <summary>
			/// Execute raw query
			/// </summary>
			/// <param name="query">raw query</param>
			/// <returns>execution result</returns>
			utility::SQLiteResult execute(const std::string& query);

		public:
			SQLiteDatabaseModel() = default;

			SQLiteDatabaseModel(const SQLiteDatabaseModel&) = delete;

			SQLiteDatabaseModel& operator = (const SQLiteDatabaseModel&) = delete;

			SQLiteDatabaseModel(SQLiteDatabaseModel&& other) noexcept = default;

			SQLiteDatabaseModel& operator = (SQLiteDatabaseModel&& other) noexcept = default;

			/// <summary>
			/// Raw SQL query
			/// </summary>
			/// <param name="query">SQL query</param>
			/// <returns>result of SELECT query, empty string otherwise</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			utility::SQLiteResult raw(const std::string& query);

			/// <summary>
			/// Create table
			/// </summary>
			/// <param name="attributes">field name - field description</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual void createTable(const std::vector<std::pair<std::string, std::string>>& attributes = {});

			/// <summary>
			/// Delete table
			/// </summary>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual void dropTable();

			/// <summary>
			/// Delete and create table
			/// </summary>
			/// <param name="attributes">field name- field description</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual void recreateTable(const std::vector<std::pair<std::string, std::string>>& attributes = {});

			/// <summary>
			/// INSERT row
			/// </summary>
			/// <param name="attributes">field name - field value</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual utility::SQLiteResult insert(const std::unordered_map<std::string, std::string>& attributes = {});

			/// <summary>
			/// UPDATE table
			/// </summary>
			/// <param name="attributes">new values</param>
			/// <param name="fieldName">for condition</param>
			/// <param name="fieldValue">for condition</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual void update(const std::unordered_map<std::string, std::string>& attributes, const std::string& fieldName, const std::string& fieldValue);

			/// <summary>
			/// Delete from table
			/// </summary>
			/// <param name="fieldName">for condition</param>
			/// <param name="fieldValue">for condition</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual void deleteQuery(const std::string& fieldName, const std::string& fieldValue);

			/// @brief Delete from table
			/// @param attributes key - value condition
			/// @exception framework::exceptions::SQLite3Exception sqlite3_errmsg
			virtual void deleteQuery(const std::unordered_map<std::string, std::string>& attributes = {});

			/// <summary>
			/// SELECT all
			/// </summary>
			/// <returns>all rows from table</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			utility::SQLiteResult selectAll();

			/// <summary>
			/// SELECT with condition
			/// </summary>
			/// <param name="fieldName">for condition</param>
			/// <param name="fieldValue">for condition</param>
			/// <returns>all rows that accept condition</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual utility::SQLiteResult selectByField(const std::string& fieldName, const std::string& fieldValue);

			/// <summary>
			/// SELECT with condition
			/// </summary>
			/// <param name="values">field name - field value</param>
			/// <returns>all rows that accept condition</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			virtual utility::SQLiteResult selectByField(const std::unordered_map<std::string, std::string>& attributes);

			virtual ~SQLiteDatabaseModel() = default;

			friend class SQLiteManager;
		};
	}
}
