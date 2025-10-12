#pragma once

#include "ITable.h"

namespace framework::interfaces
{
	class IDatabase
	{
	public:
		virtual bool contains(const char* tableName, ITable** outTable = nullptr) const = 0;
		
		virtual ITable* get(const char* tableName) const = 0;

		virtual ITable* getOrCreateTable(const char* tableName, const char* createTableQuery) = 0;

		virtual const char* getDatabaseName() const = 0;

		virtual const char* getDatabaseFileName() const = 0;

		virtual ~IDatabase() = default;
	};
}
