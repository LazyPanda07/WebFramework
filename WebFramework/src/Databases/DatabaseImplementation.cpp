#include "DatabaseImplementation.h"

#include "TableImplementation.h"
#include "Managers/DatabasesManager.h"

#include "DatabaseFactory.h"

using namespace std;

namespace framework
{
	DatabaseImplementation::DatabaseImplementation(std::shared_ptr<database::Database> database) :
		database(database)
	{

	}

	bool DatabaseImplementation::contains(const char* tableName, interfaces::ITable** outTable) const
	{
		database::Table* table = nullptr;
		bool result = database->contains(tableName, &table);

		if (outTable && result)
		{
			*outTable = new TableImplementation(table);
		}

		return result;
	}

	interfaces::ITable* DatabaseImplementation::get(const char* tableName) const
	{
		return new TableImplementation(database->get(tableName).get());
	}

	interfaces::ITable* DatabaseImplementation::createOrGetTable(const char* tableName, const char* createTableQuery)
	{
		return new TableImplementation
		(
			database::createRawTable
			(
				DatabasesManager::get().getDatabaseImplementationName(), 
				tableName, 
				database::CreateTableQuery(createTableQuery), 
				database.get()
			)
		);
	}

	void DatabaseImplementation::deleteTable(interfaces::ITable* table) const
	{
		delete table;
	}

	const char* DatabaseImplementation::getDatabaseName() const
	{
		return database->getDatabaseName().data();
	}

	const char* DatabaseImplementation::getDatabaseFileName() const
	{
		string temp = database->getDatabaseFileName();
		char* result = new char[temp.size() + 1];

		result[temp.size()] = '\0';

		temp.copy(result, temp.size());

		return result;
	}

	void DatabaseImplementation::deleteDatabaseFileName(const char* ptr) const
	{
		delete[] ptr;
	}
}
