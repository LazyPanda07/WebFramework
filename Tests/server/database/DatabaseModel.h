#pragma once

#include "SQLite3/SQLiteDatabaseModel.h"

class DatabaseModel : public framework::sqlite::SQLiteDatabaseModel
{
	DECLARE_DATABASE_AND_TABLE_NAME("test_database", "test_table");

public:
	DatabaseModel() = default;

	~DatabaseModel() = default;
};
