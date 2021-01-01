#pragma once

#include <string>

#include "sqlite3.h"

namespace framework
{
	namespace sqlite
	{
		class SQLiteDatabase
		{
		private:
			std::string dbName;
			sqlite3* db;

		public:
			SQLiteDatabase(const std::string& dbName);

			SQLiteDatabase(SQLiteDatabase&& other) noexcept;

			SQLiteDatabase(const SQLiteDatabase&) = delete;

			SQLiteDatabase& operator = (const SQLiteDatabase&) = delete;

			SQLiteDatabase& operator = (SQLiteDatabase&& other) noexcept;

			void close();

			bool isOpen();

			sqlite3* operator * ();

			~SQLiteDatabase();
		};
	}
}
