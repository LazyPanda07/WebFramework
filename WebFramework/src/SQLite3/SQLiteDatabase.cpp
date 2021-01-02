#include "SQLiteDatabase.h"

#pragma comment (lib, "sqlite3.lib")

using namespace std;

namespace framework
{
	namespace sqlite
	{
		sqlite3* SQLiteDatabase::operator * ()
		{
			return db;
		}

		SQLiteDatabase::SQLiteDatabase(const string& databaseName) :
			databaseName(databaseName)
		{
			sqlite3_open(databaseName.data(), &db);
		}

		SQLiteDatabase::SQLiteDatabase(SQLiteDatabase&& other) noexcept
		{
			(*this) = move(other);
		}

		SQLiteDatabase& SQLiteDatabase::operator = (SQLiteDatabase&& other) noexcept
		{
			this->close();

			databaseName = move(other.databaseName);
			db = other.db;

			other.db = nullptr;

			return *this;
		}

		const string& SQLiteDatabase::getDatabaseName() const
		{
			return databaseName;
		}

		void SQLiteDatabase::close()
		{
			sqlite3_close(db);

			db = nullptr;
		}

		bool SQLiteDatabase::isOpen() const
		{
			return db;
		}

		const sqlite3* const SQLiteDatabase::operator * () const
		{
			return db;
		}

		SQLiteDatabase::~SQLiteDatabase()
		{
			this->close();
		}
	}
}
