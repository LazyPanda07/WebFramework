#include "DatabasesManager.h"

using namespace std;

namespace framework
{
	DatabasesManager& DatabasesManager::get()
	{
		static DatabasesManager instance;

		return instance;
	}

	void DatabasesManager::initDatabaseImplementation(string_view databaseImplementationName)
	{
		this->databaseImplementationName = databaseImplementationName;
	}

	string_view DatabasesManager::getDatabaseImplementationName() const
	{
		return databaseImplementationName;
	}
}
