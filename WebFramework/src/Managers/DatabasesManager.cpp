#include "DatabasesManager.h"

#include <format>

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
		unique_lock<mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			it = databases.emplace(databaseName, database::createDatabase(databaseImplementationName, databaseName)).first;
		}

		return it->second;
	}

	shared_ptr<database::Database> DatabasesManager::getDatabase(string_view databaseName)
	{
		unique_lock<mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			throw runtime_error(format("Can't get database with name: {}", databaseName));
		}

		return it->second;
	}

	string_view DatabasesManager::getDatabaseImplementationName() const
	{
		return databaseImplementationName;
	}
}
