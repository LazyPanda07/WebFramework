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

		std::shared_lock<std::shared_mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			lock.unlock();

			std::unique_lock<std::shared_mutex> writeLock(databasesMutex);

			return databases.emplace(databaseName, database::createDatabase(name, databaseName)).first->second;
		}

		return it->second;
	}

	std::shared_ptr<database::Database> DatabasesManager::constructDatabase(std::string_view databaseName, std::string_view constructData, std::string_view databaseImplementationName)
	{
		auto& [name, databases] = databaseImplementationName.size() ? *std::ranges::find_if
		(
			holders,
			[databaseImplementationName](const DatabasesHolder& holder)
			{
				return holder.databaseImplementationName == databaseImplementationName;
			}
		) : holders.front();

		std::lock_guard<std::shared_mutex> lock(databasesMutex);
		
		if (auto it = databases.find(databaseName); it != databases.end())
		{
			it->second = database::createDatabase(name, constructData);
		}
		else
		{
			return databases.emplace(databaseName, database::createDatabase(name, constructData)).first->second;
		}
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

		std::shared_lock<std::shared_mutex> lock(databasesMutex);
		auto it = databases.find(databaseName);

		if (it == databases.end())
		{
			utility::logAndThrowException<logging::message::cantGetDatabase, logging::category::database>(databaseName);
		}

		return it->second;
	}
}
