#pragma once

#include "Databases/Interfaces/IDatabase.h"

#include "Framework/WebFrameworkPlatform.h"

#include "Database.h"

namespace framework
{
	class WEB_FRAMEWORK_API DatabaseImplementation : public interfaces::IDatabase
	{
	private:
		std::shared_ptr<database::Database> database;
		mutable std::vector<interfaces::ITable*> tables;

	public:
		DatabaseImplementation(std::shared_ptr<database::Database> database);

		bool contains(const char* tableName, interfaces::ITable** outTable = nullptr) const override;

		interfaces::ITable* get(const char* tableName) const override;

		interfaces::ITable* createOrGetTable(const char* tableName, const char* createTableQuery) override;

		const char* getDatabaseName() const override;

		const char* getDatabaseFileName() const override;

		void deleteDatabaseFileName(const char* ptr) const override;

		~DatabaseImplementation();
	};
}
