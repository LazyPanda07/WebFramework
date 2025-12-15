#pragma once

#include "DatabaseInterfaces/IDatabase.hpp"

#include "Table.hpp"

namespace framework
{
	class Database
	{
	private:
		interfaces::IDatabase* implementation;

	private:
		Database(interfaces::IDatabase* implementation);

	public:
		/**
		 * @brief Check if table exists in database
		 * @param tableName Name of table
		 * @param outTable Optional parameter. Also receive Table instance
		 * @return 
		 */
		bool contains(std::string_view tableName, Table* outTable = nullptr) const;

		Table getTable(std::string_view tableName) const;

		/**
		 * @brief Tries to get table from database or create if can't get
		 * @param tableName Name of table
		 * @param createTableQuery Create table query if can't get table
		 * @return 
		 */
		Table getOrCreateTable(std::string_view tableName, std::string_view createTableQuery);

		std::string_view getDatabaseName() const;

		std::string getDatabaseFileName() const;

		~Database() = default;

		friend class HttpRequest;
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
		return Table(implementation->getOrCreateTable(tableName.data(), createTableQuery.data()));
	}

	inline std::string_view Database::getDatabaseName() const
	{
		return implementation->getDatabaseName();
	}

	inline std::string Database::getDatabaseFileName() const
	{
		return implementation->getDatabaseFileName();
	}
}
