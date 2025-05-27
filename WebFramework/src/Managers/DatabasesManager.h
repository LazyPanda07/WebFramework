#pragma once

#include <string>

namespace framework
{
	class DatabasesManager
	{
	private:
		std::string databaseImplementationName;

	private:
		DatabasesManager() = default;

		~DatabasesManager() = default;

	public:
		static DatabasesManager& get();

		void initDatabaseImplementation(std::string_view databaseImplementationName);

		std::string_view getDatabaseImplementationName() const;
	};
}
