#pragma once

#include "WebFrameworkCore.h"

#include <sqlite3.h>

#ifndef __LINUX__
#pragma push_macro("max")

#undef max
#endif

namespace std
{
	template<>
	struct default_delete<sqlite3>
	{
		void operator ()(sqlite3* ptr) const
		{
			sqlite3_close(ptr);
		}
	};
}

namespace framework
{
	namespace sqlite
	{
		class SQLiteDatabaseModel;

		/// <summary>
		/// Wrapper class for sqlite3 library
		/// <para>Providing connect to sqlite3 database</para>
		/// </summary>
		class SQLiteDatabase
		{
		private:
			std::string databaseName;
			std::unique_ptr<sqlite3> db;

		private:
			sqlite3* operator * ();

		public:
			SQLiteDatabase(std::string_view databaseName);

			SQLiteDatabase(SQLiteDatabase&& other) noexcept = default;

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
			SQLiteDatabase& operator = (SQLiteDatabase&& other) noexcept = default;

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

			~SQLiteDatabase() = default;

		public:
			friend class SQLiteDatabaseModel;
		};
	}
}

#ifndef __LINUX__
#pragma pop_macro("max")
#endif
