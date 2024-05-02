#pragma once

#include "SQLite3/SQLiteDatabaseModel.h"

class DatabaseModel : public framework::sqlite::SQLiteDatabaseModel
{
public:
	DatabaseModel();

	~DatabaseModel() = default;
};
