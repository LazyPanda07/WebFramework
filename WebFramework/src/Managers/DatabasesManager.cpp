#include "DatabasesManager.h"

#include <format>

#include <DatabaseFactory.h>
#include <DatabaseUtility.h>

namespace framework
{
	DatabasesManager& DatabasesManager::get()
	{
		static DatabasesManager instance;

		return instance;
	}

	void DatabasesManager::initDatabaseImplementation(const std::vector<std::string>& databases)
	{
		for (const std::string& database : databases)
		{
			holders.emplace_back(database);
		}
	}

	std::shared_ptr<database::Database> DatabasesManager::getOrCreateDatabase(std::string_view databaseName, std::string_view databaseImplementationName)
	{
		::utility::strings::string_based_unordered_map<std::shared_ptr<database::Database>>& databases = databaseImplementationName.size() ? std::ranges::find_if
		(
			holders,
			[databaseImplementationName](const DatabasesHolder& holder)
			{
				return holder.databaseImplementationName == databaseImplementationName;
			}
		)->databases : holders.front().databases;

		std::lock_guard<std::mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			it = databases.emplace(databaseName, database::createDatabase(databaseImplementationName, databaseName)).first;
		}

		return it->second;
	}

	std::shared_ptr<database::Database> DatabasesManager::getDatabase(std::string_view databaseName, std::string_view databaseImplementationName) const
	{
		const ::utility::strings::string_based_unordered_map<std::shared_ptr<database::Database>>& databases = databaseImplementationName.size() ? std::ranges::find_if
		(
			holders,
			[databaseImplementationName](const DatabasesHolder& holder)
			{
				return holder.databaseImplementationName == databaseImplementationName;
			}
		)->databases : holders.front().databases;

		std::lock_guard<std::mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			throw std::runtime_error(std::format("Can't get database with name: {}", databaseName));
		}

		return it->second;
	}

	std::string_view DatabasesManager::getDatabaseImplementationName() const
	{
		return database::implementation::sqlite;
	}
}
