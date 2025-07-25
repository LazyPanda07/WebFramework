#pragma once

#include "DatabaseInterfaces/IDatabase.h"

#include "Table.hpp"

namespace framework
{
	class Database
	{
	private:
		interfaces::IDatabase* implementation;

	public:
		Database(interfaces::IDatabase* implementation);

		bool contains(std::string_view tableName, Table* outTable = nullptr) const;

		Table getTable(std::string_view tableName) const;

		Table getOrCreateTable(std::string_view tableName, std::string_view createTableQuery);

		std::string_view getDatabaseName() const;

		std::string getDatabaseFileName() const;

		~Database() = default;
	};
}

namespace framework
{
	inline Database::Database(interfaces::IDatabase* implementation) :
		implementation(implementation)
	{

	}

	inline bool Database::contains(std::string_view tableName, Table* outTable) const
	{
		interfaces::ITable* table = nullptr;
		bool result = implementation->contains(tableName.data(), &table);

		if (result && outTable)
		{
			*outTable = Table(table);
		}

		return result;
	}

	inline Table Database::getTable(std::string_view tableName) const
	{
		return Table(implementation->get(tableName.data()));
	}

	inline Table Database::getOrCreateTable(std::string_view tableName, std::string_view createTableQuery)
	{
		return Table(implementation->createOrGetTable(tableName.data(), createTableQuery.data()));
	}

	inline std::string_view Database::getDatabaseName() const
	{
		return implementation->getDatabaseName();
	}

	inline std::string Database::getDatabaseFileName() const
	{
		const char* temp = implementation->getDatabaseFileName();
		std::string result(temp);

		implementation->deleteDatabaseFileName(temp);

		return result;
	}
}
