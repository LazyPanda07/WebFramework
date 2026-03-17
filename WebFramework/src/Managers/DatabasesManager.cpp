#include "Managers/DatabasesManager.h"

#include <format>

#include <DatabaseFactory.h>
#include <DatabaseUtility.h>

#include "Utility/Utils.h"

namespace framework
{
	DatabasesManager::DatabasesHolder::DatabasesHolder(std::string_view databaseImplementationName) :
		databaseImplementationName(databaseImplementationName)
	{

	}

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
		auto& [name, databases]= databaseImplementationName.size() ? *std::ranges::find_if
		(
			holders,
			[databaseImplementationName](const DatabasesHolder& holder)
			{
				return holder.databaseImplementationName == databaseImplementationName;
			}
		) : holders.front();

		std::lock_guard<std::mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			it = databases.emplace(databaseName, database::createDatabase(name, databaseName)).first;
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
			utility::logAndThrowException<logging::message::cantGetDatabase, logging::category::database>(databaseName);
		}

		return it->second;
	}
}
