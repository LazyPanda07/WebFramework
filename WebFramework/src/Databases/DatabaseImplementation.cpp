#include "DatabaseImplementation.h"

#include <algorithm>

#include <DatabaseFactory.h>

#include "TableImplementation.h"
#include "Managers/DatabasesManager.h"

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
			*outTable = tables.emplace_back(new TableImplementation(table));
		}

		return result;
	}

	interfaces::ITable* DatabaseImplementation::get(const char* tableName) const
	{
		return tables.emplace_back(new TableImplementation(database->get(tableName).get()));
	}

	interfaces::ITable* DatabaseImplementation::getOrCreateTable(const char* tableName, const char* createTableQuery)
	{
		return tables.emplace_back
		(
			new TableImplementation
			(
				database::createRawTable
				(
					DatabasesManager::get().getDatabaseImplementationName(),
					tableName,
					database::CreateTableQuery(createTableQuery),
					database.get()
				)
			)
		);
	}

	const char* DatabaseImplementation::getDatabaseName() const
	{
		return database->getDatabaseName().data();
	}

	const char* DatabaseImplementation::getDatabaseFileName() const
	{
		return database->getDatabaseFileName().data();
	}

	DatabaseImplementation::~DatabaseImplementation()
	{
		std::ranges::for_each(tables, [](interfaces::ITable* table) { delete table; });
	}
}
