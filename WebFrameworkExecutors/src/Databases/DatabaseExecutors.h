#pragma once

#include "DatabaseInterfaces/IDatabase.h"

#include "TableExecutors.h"
#include "ExecutorsConstants.h"

namespace framework
{
	class EXECUTORS_API DatabaseExecutors
	{
	private:
		interfaces::IDatabase* implementation;

	public:
		DatabaseExecutors(interfaces::IDatabase* implementation);

		bool contains(std::string_view tableName, TableExecutors* outTable = nullptr) const;

		TableExecutors getTable(std::string_view tableName) const;

		TableExecutors getOrCreateTable(std::string_view tableName, std::string_view createTableQuery);

		std::string_view getDatabaseName() const;

		std::string getDatabaseFileName() const;

		~DatabaseExecutors() = default;
	};
}
