#pragma once

#include <unordered_map>
#include <memory>
#include <type_traits>

#include "SQLiteDatabaseModel.h"

namespace framework
{
	namespace sqlite
	{
		class WEB_FRAMEWORK_API SQLiteManager
		{
		private:
			std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<SQLiteDatabaseModel>>> allTables;	//database name - table name - SQLiteDatabaseModel subclass

		public:
			SQLiteManager() = default;

			/// <summary>
			/// Get instance of SQLiteDatabaseModel subclass for database operations
			/// </summary>
			/// <typeparam name="SQLiteDatabaseModelSubclass">subclass of SQLiteDatabaseModel</typeparam>
			/// <typeparam name="...Args">arguments for constructor if needs to create new instance</typeparam>
			/// <param name="databaseName">name of database</param>
			/// <param name="tableName">name of table</param>
			/// <param name="...args">arguments for constructor if needs to create new instance</param>
			/// <returns>instance of SQLiteDatabaseModel subclass</returns>
			template<typename SQLiteDatabaseModelSubclass, typename... Args>
			std::unique_ptr<SQLiteDatabaseModel>& get(const std::string& databaseName, const std::string& tableName, Args&&... args);

			~SQLiteManager() = default;
		};

		template<typename SQLiteDatabaseModelSubclass, typename... Args>
		std::unique_ptr<SQLiteDatabaseModel>& SQLiteManager::get(const std::string& databaseName, const std::string& tableName, Args&&... args)
		{
			static_assert(std::is_base_of_v<SQLiteDatabaseModel, SQLiteDatabaseModelSubclass>, "SQLiteDatabaseModelSubclass must be subclass of SQLiteDatabaseModel");

			auto database = allTables.find(databaseName);

			if (database != allTables.end())
			{
				auto table = database->second.find(tableName);

				if (table != database->second.end())
				{
					return table->second;
				}
				else
				{
					return allTables[databaseName].emplace(std::make_pair(tableName, std::make_unique<SQLiteDatabaseModelSubclass>(std::forward<Args>(args)...))).first->second;
				}
			}
			else
			{
				allTables.emplace(std::make_pair(databaseName, std::unordered_map<std::string, std::string>()));

				return allTables[databaseName].emplace(std::make_pair(tableName, std::make_unique<SQLiteDatabaseModelSubclass>(std::forward<Args>(args)...))).first->second;
			}
		}
	}
}
