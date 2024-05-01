#include "SQLiteDatabase.h"

#include "Exceptions/SQLite3Exception.h"

using namespace std;

namespace framework
{
	namespace sqlite
	{
		sqlite3* SQLiteDatabase::operator * ()
		{
			return db.get();
		}

		SQLiteDatabase::SQLiteDatabase(string_view databaseName) :
			databaseName(databaseName)
		{
			sqlite3* connection = nullptr;

			if (sqlite3_open(databaseName.data(), &connection) != SQLITE_OK)
			{
				throw exceptions::SQLite3Exception(format("Can't open {} database", databaseName));
			}

			db = unique_ptr<sqlite3>(connection);
		}
		
		const string& SQLiteDatabase::getDatabaseName() const
		{
			return databaseName;
		}

		void SQLiteDatabase::close()
		{
			db.reset();
		}

		bool SQLiteDatabase::isOpen() const
		{
			return static_cast<bool>(db);
		}

		const sqlite3* const SQLiteDatabase::operator * () const
		{
			return db.get();
		}
	}
}
