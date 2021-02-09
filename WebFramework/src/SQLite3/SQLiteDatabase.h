#pragma once

#if defined(DATABASES_DLL) || defined(ALL_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // DATABASES_DLL

#include <string>

#include "sqlite3.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace sqlite
	{
		/// <summary>
		/// Forward declaration
		/// </summary>
		class SQLiteDatabaseModel;

		/// <summary>
		/// <para>Wrapper class for sqlite3 library</para>
		/// <para>Providing connect to sqlite3 database</para>
		/// </summary>
		class WEB_FRAMEWORK_API SQLiteDatabase
		{
		private:
			std::string databaseName;
			sqlite3* db;

		private:
			/// <summary>
			/// Getter for db
			/// </summary>
			/// <returns>pointer to sqlite3 handle connection</returns>
			sqlite3* operator * ();

		public:
			/// <summary>
			/// Create database
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			SQLiteDatabase(const std::string& databaName);

			/// <summary>
			/// Move another database
			/// </summary>
			/// <param name="other">another SQLiteDatabase instance</param>
			SQLiteDatabase(SQLiteDatabase&& other) noexcept;

			/// <summary>
			/// Can't copy connection handle from another database
			/// </summary>
			SQLiteDatabase(const SQLiteDatabase&) = delete;

			/// <summary>
			/// Can't copy connection handle from another database
			/// </summary>
			SQLiteDatabase& operator = (const SQLiteDatabase&) = delete;

			/// <summary>
			/// Move another database
			/// </summary>
			/// <param name="other">another SQLiteDatabase instance</param>
			SQLiteDatabase& operator = (SQLiteDatabase&& other) noexcept;

			/// <summary>
			/// Getter for databaseName
			/// </summary>
			/// <returns>database name</returns>
			const std::string& getDatabaseName() const;

			/// <summary>
			/// Close connection with database
			/// </summary>
			void close();

			/// <summary>
			/// Is database connection open
			/// </summary>
			/// <returns>true if database connection is open, false otherwise</returns>
			bool isOpen() const;

			/// <summary>
			/// Getter for db
			/// </summary>
			/// <returns>pointer to sqlite3 handle connection</returns>
			const sqlite3* const operator * () const;

			~SQLiteDatabase();

		public:
			friend class SQLiteDatabaseModel;
		};
	}
}
