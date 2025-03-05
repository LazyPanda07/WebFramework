#include "SQLiteManager.h"

using namespace std;

namespace framework::sqlite
{
	SQLiteManager::Database::Database(const string& databaseName) :
		database(make_shared<SQLiteDatabase>(databaseName))
	{

	}
}
