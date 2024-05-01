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
			std::unordered_map<std::string, std::unordered_map<std::string, smartPointer<SQLiteDatabaseModel>>> allTables;	// database name - table name - SQLiteDatabaseModel subclass

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
			template<std::derived_from<SQLiteDatabaseModel> SQLiteDatabaseModelSubclass, typename... Args>
			smartPointer<SQLiteDatabaseModel>& get(const std::string& databaseName, std::string_view tableName, Args&&... args);

			~SQLiteManager() = default;
		};

		template<std::derived_from<SQLiteDatabaseModel> SQLiteDatabaseModelSubclass, typename... Args>
		smartPointer<SQLiteDatabaseModel>& SQLiteManager::get(const std::string& databaseName, std::string_view tableName, Args&&... args)
		{
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
					return allTables[databaseName].emplace(std::make_pair(tableName, utility::make_smartPointer<SQLiteDatabaseModelSubclass>(std::forward<Args>(args)...))).first->second;
				}
			}
			else
			{
				allTables.emplace(std::make_pair(databaseName, std::unordered_map<std::string, smartPointer<SQLiteDatabaseModel>>()));

				return allTables[databaseName].emplace(std::make_pair(tableName, utility::make_smartPointer<SQLiteDatabaseModelSubclass>(std::forward<Args>(args)...))).first->second;
			}
		}
	}
}
