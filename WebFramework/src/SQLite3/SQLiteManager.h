#pragma once

#include "headers.h"

#include "SQLiteDatabaseModel.h"

namespace framework
{
	namespace sqlite
	{
		class WEB_FRAMEWORK_API SQLiteManager
		{
		private:
			std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<SQLiteDatabaseModel>>> allTables;
			std::shared_mutex mutex;

		public:
			SQLiteManager() = default;

			template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
			std::shared_ptr<SQLiteDatabaseModel> create(const std::string& databaseName, const std::string& tableName, Args&&... args);

			template<std::derived_from<SQLiteDatabaseModel> T>
			std::shared_ptr<SQLiteDatabaseModel> get(const std::string& databaseName, const std::string& tableName);

			~SQLiteManager() = default;
		};

		template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<SQLiteDatabaseModel> SQLiteManager::create(const std::string& databaseName, const std::string& tableName, Args&&... args)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			auto database = allTables.find(databaseName);

			if (database != allTables.end())
			{
				auto& tables = database->second;
				auto table = tables.find(tableName);

				if (table != tables.end())
				{
					return std::dynamic_pointer_cast<T>(table->second);
				}

				std::shared_ptr<T> model = std::make_shared(std::forward<Args>(args)...);

				tables[tableName] = model;

				return model;
			}
			else
			{
				std::shared_ptr<T> model = std::make_shared(std::forward<Args>(args)...);
				auto& tables = allTables.emplace(databaseName).first->second;

				tables[tableName] = model;

				return model;
			}
		}

		template<std::derived_from<SQLiteDatabaseModel> T>
		std::shared_ptr<SQLiteDatabaseModel> SQLiteManager::get(const std::string& databaseName, const std::string& tableName)
		{
			std::shared_lock<std::shared_mutex> lock(mutex);
			auto database = allTables.find(databaseName);

			if (database == allTables.end())
			{
				return nullptr;
			}

			auto& tables = database->second;
			auto table = tables.find(tableName);

			if (table == tables.end())
			{
				return nullptr;
			}

			return table->second;
		}
	}
}
