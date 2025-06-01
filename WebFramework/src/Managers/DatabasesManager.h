#pragma once

#include <string>

#include "Strings.h"
#include "Database.h"

namespace framework
{
	class DatabasesManager
	{
	private:
		std::string databaseImplementationName;
		::utility::strings::string_based_unordered_map<std::shared_ptr<database::Database>> databases;

	private:
		DatabasesManager() = default;

		~DatabasesManager() = default;

	public:
		static DatabasesManager& get();

		void initDatabaseImplementation(std::string_view databaseImplementationName);

		std::shared_ptr<database::Database> getOrCreateDatabase(std::string_view databaseName);

		std::string_view getDatabaseImplementationName() const;
	};
}
