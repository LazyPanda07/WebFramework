#pragma once

#include "Databases/Interfaces/IDatabase.h"

#include "Table.h"
#include "ExecutorsConstants.h"

namespace framework
{
	class EXECUTORS_API Database
	{
	private:
		interfaces::IDatabase* implementation;

	public:
		Database(interfaces::IDatabase* implementation);

		bool contains(std::string_view tableName, Table* outTable = nullptr) const;

		Table get(std::string_view tableName) const;

		Table createOrGetTable(std::string_view tableName, std::string_view createTableQuery);

		std::string_view getDatabaseName() const;

		std::string getDatabaseFileName() const;

		~Database() = default;
	};
}
