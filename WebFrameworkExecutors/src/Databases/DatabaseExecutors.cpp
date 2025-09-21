#include "DatabaseExecutors.h"

namespace framework
{
	DatabaseExecutors::DatabaseExecutors(interfaces::IDatabase* implementation) :
		implementation(implementation)
	{

	}

	bool DatabaseExecutors::contains(std::string_view tableName, TableExecutors* outTable) const
	{
		interfaces::ITable* table = nullptr;
		bool result = implementation->contains(tableName.data(), &table);

		if (result && outTable)
		{
			*outTable = TableExecutors(table);
		}

		return result;
	}

	TableExecutors DatabaseExecutors::getTable(std::string_view tableName) const
	{
		return TableExecutors(implementation->get(tableName.data()));
	}

	TableExecutors DatabaseExecutors::getOrCreateTable(std::string_view tableName, std::string_view createTableQuery)
	{
		return TableExecutors(implementation->getOrCreateTable(tableName.data(), createTableQuery.data()));
	}

	std::string_view DatabaseExecutors::getDatabaseName() const
	{
		return implementation->getDatabaseName();
	}

	std::string DatabaseExecutors::getDatabaseFileName() const
	{
		return implementation->getDatabaseFileName();
	}
}
