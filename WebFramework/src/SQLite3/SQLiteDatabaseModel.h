#pragma once

#include <string>
#include <vector>
#include <map>
#include <shared_mutex>

#include "SQLiteDatabase.h"
#include "SQLiteResult.h"

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
			enum class queryType
			{
				read,
				write
			};

		private:
			std::string tableName;
			SQLiteDatabase db;
			std::shared_mutex readWriteMutex;

		private:
			/// <summary>
			/// Check that string represent number
			/// </summary>
			/// <param name="source">attribute value</param>
			/// <returns>true if source is correct number, false otherwise</returns>
			static bool isNumber(const std::string& source);

		private:
			/// <summary>
			/// Execute raw query
			/// </summary>
			/// <param name="query">raw query</param>
			/// <returns>execution result</returns>
			utility::SQLiteResult executeQuery(const std::string& query);

		public:
			/// <summary>
			/// Specify table for database
			/// </summary>
			/// <param name="tableName">name of table</param>
			/// <param name="db">temporary database object</param>
			SQLiteDatabaseModel(const std::string& tableName, SQLiteDatabase&& db);

			/// <summary>
			/// Can't copy model for specific table
			/// </summary>
			/// <param name="">other SQLiteDatabaseModel instance</param>
			SQLiteDatabaseModel(const SQLiteDatabaseModel&) = delete;

			/// <summary>
			/// Can't copy model for specific table
			/// </summary>
			/// <param name="">other SQLiteDatabaseModel instance</param>
			SQLiteDatabaseModel& operator = (const SQLiteDatabaseModel&) = delete;

			/// <summary>
			/// Move another SQLiteDatabaseModel
			/// </summary>
			/// <param name="other">another SQLiteDatabaseModel instance</param>
			SQLiteDatabaseModel(SQLiteDatabaseModel&& other) noexcept;

			/// <summary>
			/// Move another SQLiteDatabaseModel
			/// </summary>
			/// <param name="other">another SQLiteDatabaseModel instance</param>
			SQLiteDatabaseModel& operator = (SQLiteDatabaseModel&& other) noexcept;

			/// <summary>
			/// Raw SQL query
			/// </summary>
			/// <param name="query">SQL query</param>
			/// <returns>result of SELECT query, empty string otherwise</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			/// <exception cref="std::invalid_argument">wrong type</exception>
			utility::SQLiteResult rawQuery(const std::string& query, queryType type);

			/// <summary>
			/// Create table
			/// </summary>
			/// <param name="attributes">field name - field description</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			void createTableQuery(const std::vector<std::pair<std::string, std::string>>& attributes);

			/// <summary>
			/// Delete table
			/// </summary>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			void dropTableQuery();

			/// <summary>
			/// Delete and create table
			/// </summary>
			/// <param name="attributes">field name- field description</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			void recreateTableQuery(const std::vector<std::pair<std::string, std::string>>& attributes);

			/// <summary>
			/// INSERT row
			/// </summary>
			/// <param name="attributes">field name - field value</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			utility::SQLiteResult insertQuery(const std::map<std::string, std::string>& attributes);

			/// <summary>
			/// UPDATE table
			/// </summary>
			/// <param name="attributes">new values</param>
			/// <param name="fieldName">for condition</param>
			/// <param name="fieldValue">for condition</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			void updateQuery(const std::map<std::string, std::string>& attributes, const std::string& fieldName, const std::string& fieldValue);

			/// <summary>
			/// Delete from table
			/// </summary>
			/// <param name="fieldName">for condition</param>
			/// <param name="fieldValue">for condition</param>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			void deleteQuery(const std::string& fieldName, const std::string& fieldValue);

			/// @brief Delete from table
			/// @param attributes key - value condition
			/// @exception framework::exceptions::SQLite3Exception sqlite3_errmsg
			void deleteQuery(const std::map<std::string, std::string>& attributes);

			/// <summary>
			/// SELECT all
			/// </summary>
			/// <returns>all rows from table</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			utility::SQLiteResult selectAllQuery();

			/// <summary>
			/// SELECT with condition
			/// </summary>
			/// <param name="fieldName">for condition</param>
			/// <param name="fieldValue">for condition</param>
			/// <returns>all rows that accept condition</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			utility::SQLiteResult selectByFieldQuery(const std::string& fieldName, const std::string& fieldValue);

			/// <summary>
			/// SELECT with condition
			/// </summary>
			/// <param name="values">field name - field value</param>
			/// <returns>all rows that accept condition</returns>
			/// <exception cref="framework::exceptions::SQLite3Exception">sqlite3_errmsg</exception>
			utility::SQLiteResult selectByFieldQuery(const std::map<std::string, std::string>& attributes);

			/// <summary>
			/// Getter for tableName
			/// </summary>
			/// <returns>tableName</returns>
			const std::string& getTableName() const;

			~SQLiteDatabaseModel() = default;
		};
	}
}
