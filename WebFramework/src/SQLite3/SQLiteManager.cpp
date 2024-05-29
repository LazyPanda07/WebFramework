#include "SQLiteManager.h"

using namespace std;

namespace framework
{
    namespace sqlite
    {
        SQLiteManager::Database::Database(const string& databaseName) :
			database(make_shared<SQLiteDatabase>(databaseName))
		{

		}
    }
}
