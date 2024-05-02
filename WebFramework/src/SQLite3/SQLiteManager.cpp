#include "SQLiteManager.h"

namespace framework
{
    namespace sqlite
    {
        SQLiteManager::Database::Database(const std::string& databaseName) :
			database(std::make_shared<SQLiteDatabase>(databaseName))
		{

		}
    }
}
