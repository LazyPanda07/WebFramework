#include "DatabaseExecutors.h"

using namespace std;

namespace framework
{
	DatabaseExecutors::DatabaseExecutors(interfaces::IDatabase* implementation) :
		implementation(implementation)
	{

	}

	bool DatabaseExecutors::contains(string_view tableName, TableExecutors* outTable) const
	{
		interfaces::ITable* table = nullptr;
		bool result = implementation->contains(tableName.data(), &table);

		if (result && outTable)
		{
			*outTable = TableExecutors(table);
		}

		return result;
	}

	TableExecutors DatabaseExecutors::getTable(string_view tableName) const
	{
		return TableExecutors(implementation->get(tableName.data()));
	}

	TableExecutors DatabaseExecutors::getOrCreateTable(string_view tableName, string_view createTableQuery)
	{
		return TableExecutors(implementation->createOrGetTable(tableName.data(), createTableQuery.data()));
	}

	string_view DatabaseExecutors::getDatabaseName() const
	{
		return implementation->getDatabaseName();
	}

	string DatabaseExecutors::getDatabaseFileName() const
	{
		const char* temp = implementation->getDatabaseFileName();
		string result(temp);

		implementation->deleteDatabaseFileName(temp);

		return result;
	}
}
