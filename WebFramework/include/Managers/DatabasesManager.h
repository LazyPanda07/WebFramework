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
		public:
			std::string databaseImplementationName;
			::utility::strings::string_based_unordered_map<std::shared_ptr<database::Database>> databases;

		public:
			DatabasesHolder(std::string_view databaseImplementationName);

			~DatabasesHolder() = default;
		};

	private:
		std::vector<DatabasesHolder> holders;
		mutable std::mutex databasesMutex;

	public:
		DatabasesManager() = default;

		void initDatabaseImplementation(const std::vector<std::string>& databases);

		std::shared_ptr<database::Database> getOrCreateDatabase(std::string_view databaseName, std::string_view databaseImplementationName);
		
		std::shared_ptr<database::Database> getDatabase(std::string_view databaseName, std::string_view databaseImplementationName) const;

		~DatabasesManager() = default;
	};
}
