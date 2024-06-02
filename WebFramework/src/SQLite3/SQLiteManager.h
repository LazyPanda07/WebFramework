#pragma once

#include "WebFrameworkCore.h"

#include "SQLiteDatabaseModel.h"

namespace framework
{
	namespace sqlite
	{
		class WEB_FRAMEWORK_API SQLiteManager
		{
		private:
			class WEB_FRAMEWORK_API Database
			{
			private:
				class WEB_FRAMEWORK_API ModelsData
				{
				private:
					std::vector<std::pair<std::shared_ptr<SQLiteDatabaseModel>, size_t>> data;

				public:
					ModelsData() = default;

					template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
					std::shared_ptr<T> add(std::shared_ptr<SQLiteDatabase> database, Args&&... args);

					template<std::derived_from<SQLiteDatabaseModel> T>
					std::shared_ptr<T> get() const;

					~ModelsData() = default;
				};

			private:
				std::shared_ptr<SQLiteDatabase> database;
				std::unordered_map<std::string_view, ModelsData> models;
				mutable std::shared_mutex mutex;

			public:
				Database(const std::string& databaseName);

				template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
				std::shared_ptr<T> add(Args&&... args);

				template<std::derived_from<SQLiteDatabaseModel> T>
				std::shared_ptr<T> get() const;

				~Database() = default;
			};

		private:
			std::unordered_map<std::string_view, std::unique_ptr<Database>> databases;
			mutable std::shared_mutex mutex;

		public:
			SQLiteManager() = default;

			template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
			std::shared_ptr<T> add(Args&&... args);

			template<std::derived_from<SQLiteDatabaseModel> T>
			std::shared_ptr<T> get() const;

			~SQLiteManager() = default;

			friend class SQLiteDatabaseModel;
		};

		template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> SQLiteManager::Database::ModelsData::add(std::shared_ptr<SQLiteDatabase> database, Args&&... args)
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

			memset(model, 0, sizeof(T));

			model->databaseConstructor = database;

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

		template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> SQLiteManager::Database::add(Args&&... args)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			ModelsData* data = nullptr;

			if (auto modelsData = models.find(T::tableName); modelsData != models.end())
			{
				data = &modelsData->second;
			}
			else
			{
				modelsData = models.emplace(T::tableName, ModelsData()).first;

				data = &modelsData->second;
			}

			return data->add<T>(database, std::forward<Args>(args)...);
		}

		template<std::derived_from<SQLiteDatabaseModel> T>
		std::shared_ptr<T> SQLiteManager::Database::get() const
		{
			std::shared_lock<std::shared_mutex> lock(mutex);
			
			if (auto modelsData = models.find(T::tableName); modelsData != models.end())
			{
				const ModelsData& data = modelsData->second;

				return data.get<T>();
			}

			return nullptr;
		}

		template<std::derived_from<SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> SQLiteManager::add(Args&&... args)
		{
			static_assert(T::databaseName.size(), "SQLiteDatabaseModel::databaseName is empty");
			static_assert(T::tableName.size(), "SQLiteDatabaseModel::tableName is empty");

			std::unique_lock<std::shared_mutex> lock(mutex);
			std::unique_ptr<Database>* database = nullptr;

			if (auto it = databases.find(T::databaseName); it != databases.end())
			{
				database = &it->second;
			}
			else
			{
				it = databases.emplace(T::databaseName, std::make_unique<Database>(std::string(T::databaseName))).first;

				database = &it->second;
			}

			return (*database)->add<T>(std::forward<Args>(args)...);
		}

		template<std::derived_from<SQLiteDatabaseModel> T>
		std::shared_ptr<T> SQLiteManager::get() const
		{
			const Database* database = nullptr;

			{
				std::shared_lock<std::shared_mutex> lock(mutex);
				
				if (auto it = databases.find(T::databaseName); it != databases.end())
				{
					database = &(*it->second);
				}
			}
			
			return database ? database->get<T>() : nullptr;
		}
	}
}
