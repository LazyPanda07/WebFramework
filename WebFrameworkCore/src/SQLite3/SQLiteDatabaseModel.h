#pragma once

#include "SQLiteDatabase.h"
#include "SQLiteResult.h"

#define DECLARE_DATABASE_NAME(DatabaseName) public: static constexpr std::string_view databaseName = DatabaseName; inline std::string_view getDatabaseName() const override { return databaseName; }
#define DECLARE_TABLE_NAME(TableName) public: static constexpr std::string_view tableName = TableName; inline std::string_view getTableName() const override { return tableName; }
#define DECLARE_DATABASE_AND_TABLE_NAME(DatabaseName, TableName) DECLARE_DATABASE_NAME(DatabaseName); \
                                                                 DECLARE_TABLE_NAME(TableName);

namespace framework::sqlite
{
	/// <summary>
	/// Providing SELECT, INSERT, UPDATE, DELETE or raw queries for SQLiteDatabase
	/// </summary>
	class WEB_FRAMEWORK_CORE_API SQLiteDatabaseModel
	{
	public:
		static constexpr std::string_view databaseName = "";
		static constexpr std::string_view tableName = "";

	protected:
		std::shared_ptr<SQLiteDatabase> database;

	private:
		std::shared_ptr<SQLiteDatabase> databaseConstructor;

	protected:
		/**
		 * @brief Check that string represent number
		 * @param source Attribute value
		 * @return true if source is correct number, false otherwise
		 */
		static bool isNumber(std::string_view source);

		/**
		 * @brief If source is string surrounds it with quotes
		 * @param source Attribute value
		 * @return
		 */
		static std::string convertToValue(std::string_view source);

	protected:
		/// <summary>
		/// Execute raw query
		/// </summary>
		/// <param name="query">raw query</param>
		/// <returns>execution result</returns>
		utility::SQLiteResult execute(const std::string& query);

	public:
		SQLiteDatabaseModel();

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

		virtual std::string_view getDatabaseName() const = 0;

		virtual std::string_view getTableName() const = 0;

		virtual ~SQLiteDatabaseModel() = default;

		friend class SQLiteManager;
	};
}
