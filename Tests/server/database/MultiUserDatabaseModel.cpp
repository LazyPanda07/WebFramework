#include "MultiUserDatabaseModel.h"

#include <iostream>

MultiUserDatabaseModel::MultiUserDatabaseModel()
{
	this->createTable
	(
		{
			{ "id", "INTEGER PRIMARY KEY AUTOINCREMENT" },
			{ "user_id", "TEXT NOT NULL" },
			{ "data", "TEXT NOT NULL" }
		}
	);
}
