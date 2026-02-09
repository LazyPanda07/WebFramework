#include "DatabasesManager.h"

#include <format>

#include <DatabaseFactory.h>

namespace framework
{
	DatabasesManager& DatabasesManager::get()
	{
		static DatabasesManager instance;

		return instance;
	}

	void DatabasesManager::initDatabaseImplementation(const std::vector<std::string>& databases)
	{
		databaseImplementationName = databases.front();
	}

	std::shared_ptr<database::Database> DatabasesManager::getOrCreateDatabase(std::string_view databaseName)
	{
		std::lock_guard<std::mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			it = databases.emplace(databaseName, database::createDatabase(databaseImplementationName, databaseName)).first;
		}

		return it->second;
	}

	std::shared_ptr<database::Database> DatabasesManager::getDatabase(std::string_view databaseName)
	{
		std::lock_guard<std::mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			throw std::runtime_error(format("Can't get database with name: {}", databaseName));
		}

		return it->second;
	}

	std::string_view DatabasesManager::getDatabaseImplementationName() const
	{
		return databaseImplementationName;
	}
}
