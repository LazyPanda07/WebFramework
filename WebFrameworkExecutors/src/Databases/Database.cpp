#include "Database.h"

using namespace std;

namespace framework
{
	Database::Database(interfaces::IDatabase* implementation) :
		implementation(implementation)
	{

	}

	bool Database::contains(string_view tableName, Table* outTable) const
	{
		interfaces::ITable* table = nullptr;
		bool result = implementation->contains(tableName.data(), &table);

		if (result && outTable)
		{
			*outTable = Table(table);
		}

		return result;
	}

	Table Database::get(string_view tableName) const
	{
		return Table(implementation->get(tableName.data()));
	}

	Table Database::createOrGetTable(string_view tableName, string_view createTableQuery)
	{
		return Table(implementation->createOrGetTable(tableName.data(), createTableQuery.data()));
	}

	string_view Database::getDatabaseName() const
	{
		return implementation->getDatabaseName();
	}

	string Database::getDatabaseFileName() const
	{
		const char* temp = implementation->getDatabaseFileName();
		string result(temp);

		implementation->deleteDatabaseFileName(temp);

		return result;
	}
}
