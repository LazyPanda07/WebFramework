#include "SQLiteDatabase.h"

#pragma comment (lib, "sqlite3.lib")

using namespace std;

namespace framework
{
	namespace sqlite
	{
		SQLiteDatabase::SQLiteDatabase(const string& dbName) :
			dbName(dbName)
		{
			sqlite3_open(dbName.data(), &db);
		}

		SQLiteDatabase::SQLiteDatabase(SQLiteDatabase&& other) noexcept :
			dbName(move(other.dbName)),
			db(other.db)
		{
			other.db = nullptr;
		}

		SQLiteDatabase& SQLiteDatabase::operator = (SQLiteDatabase&& other) noexcept
		{
			dbName = move(other.dbName);
			db = other.db;

			other.db = nullptr;

			return *this;
		}

		sqlite3* SQLiteDatabase::operator * ()
		{
			return db;
		}

		void SQLiteDatabase::close()
		{
			sqlite3_close(db);

			db = nullptr;
		}

		bool SQLiteDatabase::isOpen()
		{
			return db;
		}

		SQLiteDatabase::~SQLiteDatabase()
		{
			this->close();
		}
	}
}
