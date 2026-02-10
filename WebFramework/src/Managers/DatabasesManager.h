#pragma once

#include <string>

#include <Strings.h>
#include <Database.h>

namespace framework
{
	class DatabasesManager
	{
	private:
		struct DatabasesHolder
		{
			std::string databaseImplementationName;
			::utility::strings::string_based_unordered_map<std::shared_ptr<database::Database>> databases;
		};

	private:
		std::vector<DatabasesHolder> holders;
		mutable std::mutex databasesMutex;

	private:
		DatabasesManager() = default;

		~DatabasesManager() = default;

	public:
		static DatabasesManager& get();

		void initDatabaseImplementation(const std::vector<std::string>& databases);

		std::shared_ptr<database::Database> getOrCreateDatabase(std::string_view databaseName, std::string_view databaseImplementationName);
		
		std::shared_ptr<database::Database> getDatabase(std::string_view databaseName, std::string_view databaseImplementationName) const;

		std::string_view getDatabaseImplementationName() const;
	};
}
