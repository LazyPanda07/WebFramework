#include <iostream>

#include "WebFramework.h"
#include "SQLite3/SQLiteDatabaseModel.h"

#pragma comment (lib, "WebFramework.lib")

using namespace std;

class Model : public framework::sqlite::SQLiteDatabaseModel
{
private:

public:
	Model() :
		SQLiteDatabaseModel
		(
			"users",
			framework::sqlite::SQLiteDatabase
			(
				"Database.sqlite3"
			)
		)
	{
		SQLiteDatabaseModel::createTableQuery({ { "id", "INTEGER PRIMARY KEY AUTOINCREMENT" }, { "login", "TEXT NOT NULL" } });
	}
};

class Test : public framework::BaseStatelessExecutor
{
public:
	void init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings) override
	{

	}

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override
	{
		auto& model = request.getDatabaseModelInstance<Model>("Database.sqlite3", "users");

		model->insertQuery({ { "login", "rofl" } });

		framework::sqlite::utility::SQLiteResult result = model->selectAllQuery();

		for (const auto& i : result)
		{
			for (const auto& j : i)
			{
				cout << "Column: " << j.first << '\t' << "Value: " << j.second << endl;
			}
		}

		response.addBody
		(
			"<h1>Loaded</h1>"
		);
	}
};

CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(Test)

int main(int argc, char** argv)
{
	try
	{
		framework::WebFramework webFramework("web_framework_settings.ini");
		string s;

		webFramework.startServer();

		while (cin >> s)
		{

		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
