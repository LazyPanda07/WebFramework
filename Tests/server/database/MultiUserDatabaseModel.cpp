#include "MultiUserDatabaseModel.h"

#include <iostream>

MultiUserDatabaseModel::MultiUserDatabaseModel()
{
	framework::sqlite::utility::SQLiteResult result = this->selectAll();

	std::cout << "Select all: " << result.size() << std::endl;
}
