#include "DatabasesManager.h"

#include "DatabaseFactory.h"

using namespace std;

namespace framework
{
	DatabasesManager& DatabasesManager::get()
	{
		static DatabasesManager instance;

		return instance;
	}

	void DatabasesManager::initDatabaseImplementation(string_view databaseImplementationName)
	{
		this->databaseImplementationName = databaseImplementationName;
	}

	shared_ptr<database::Database> DatabasesManager::getOrCreateDatabase(string_view databaseName)
	{
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			it = databases.emplace(databaseName, database::createDatabase(databaseImplementationName, databaseName)).first;
		}

		return it->second;
	}

	string_view DatabasesManager::getDatabaseImplementationName() const
	{
		return databaseImplementationName;
	}
}
