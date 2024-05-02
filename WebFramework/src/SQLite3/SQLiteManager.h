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
			class Database
			{
			private:
				class ModelsData
				{
				private:
					std::vector<std::pair<std::shared_ptr<SQLiteDatabaseModel>, size_t>> data;

				public:
					ModelsData() = default;

					template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
					std::shared_ptr<T> add(const std::shared_ptr<SQLiteDatabase>& database, Args&&... args);

					template<std::derived_from<SQLiteDatabaseModel> T>
					std::shared_ptr<T> get() const;

					~ModelsData() = default;
				};

			private:
				std::shared_ptr<SQLiteDatabase> database;
				std::unordered_map<std::string, ModelsData> models;
				mutable std::shared_mutex mutex;

			public:
				Database(const std::string& databaseName);

				template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
				std::shared_ptr<T> add(const std::string& tableName, Args&&... args);

				template<std::derived_from<SQLiteDatabaseModel> T>
				std::shared_ptr<T> get(const std::string& tableName) const;

				~Database() = default;
			};

		private:
			std::unordered_map<std::string, std::unique_ptr<Database>> databases;
			mutable std::shared_mutex mutex;

		public:
			SQLiteManager() = default;

			/**
			 * @brief Add model
			 * @tparam ...Args 
			 * @tparam T 
			 * @param databaseName 
			 * @param tableName 
			 * @param ...args Constructor arguments
			 * @return 
			 */
			template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
			std::shared_ptr<T> add(const std::string& databaseName, const std::string& tableName, Args&&... args);

			/**
			 * @brief Get model
			 * @tparam T 
			 * @param databaseName 
			 * @param tableName 
			 * @return nullptr if model is not created
			 */
			template<std::derived_from<SQLiteDatabaseModel> T>
			std::shared_ptr<T> get(const std::string& databaseName, const std::string& tableName) const;

			~SQLiteManager() = default;

			friend class SQLiteDatabaseModel;
		};

		template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> SQLiteManager::Database::ModelsData::add(const std::shared_ptr<SQLiteDatabase>& database, Args&&... args)
		{
			size_t typeHash = typeid(T).hash_code();

			for (const auto& [model, hash] : data)
			{
				if (typeHash == hash)
				{
					return std::dynamic_pointer_cast<T>(model);
				}
			}

			T* model = static_cast<T*>(malloc(sizeof(T)));

			model->database = database;

			model = new (model)(T)(std::forward<Args>(args)...);

			return std::dynamic_pointer_cast<T>(data.emplace_back(std::shared_ptr<T>(model), typeHash).first);
		}
		
		template<std::derived_from<SQLiteDatabaseModel> T>
		std::shared_ptr<T> SQLiteManager::Database::ModelsData::get() const
		{
			size_t typeHash = typeid(T).hash_code();

			for (const auto& [model, hash] : data)
			{
				if (typeHash == hash)
				{
					return std::dynamic_pointer_cast<T>(model);
				}
			}

			return nullptr;
		}

		SQLiteManager::Database::Database(const std::string& databaseName) :
			database(std::make_shared<SQLiteDatabase>(databaseName))
		{

		}

		template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> SQLiteManager::Database::add(const std::string& tableName, Args&&... args)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			ModelsData* data = nullptr;

			if (auto modelsData = models.find(tableName); modelsData != models.end())
			{
				data = &modelsData->second;
			}
			else
			{
				modelsData = models.emplace(tableName).first;

				data = &modelsData->second;
			}

			return data->add<T>(std::forward<Args>(args)...);
		}

		template<std::derived_from<SQLiteDatabaseModel> T>
		std::shared_ptr<T> SQLiteManager::Database::get(const std::string& tableName) const
		{
			std::shared_lock<std::shared_mutex> lock(mutex);
			
			if (auto modelsData = models.find(tableName); modelsData != models.end())
			{
				return modelsData->second.get<T>();
			}

			return nullptr;
		}

		template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> SQLiteManager::add(const std::string& databaseName, const std::string& tableName, Args&&... args)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			std::unique_ptr<Database>* database = nullptr;

			if (auto it = databases.find(databaseName); it != databases.end())
			{
				database = &it->second;
			}
			else
			{
				it = databases.emplace(databaseName, std::make_unique<Database>(database)).first;

				database = &it->second;
			}

			return (*database)->add(tableName, std::forward<Args>(args)...);
		}

		template<std::derived_from<SQLiteDatabaseModel> T>
		std::shared_ptr<T> SQLiteManager::get(const std::string& databaseName, const std::string& tableName) const
		{
			Database* database = nullptr;

			{
				std::shared_lock<std::shared_mutex> lock(mutex);
				
				if (auto it = databases.find(databaseName); it != databases.end())
				{
					database = &(*it->second);
				}
			}
			
			return database ? database->get(tableName) : nullptr;
		}
	}
}
