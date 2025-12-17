#include "CRUDExecutor.h"

#include <random>
#include <ctime>

#include <Utility/WebFrameworkUtility.hpp>

void CRUDExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Database database = request.getDatabase("test_database");
	framework::Table table = database.getTable("test_table");
	framework::SqlResult result = table.execute("SELECT * FROM test_table WHERE name = ?", framework::utility::database::makeSQLValues("glue"));
	std::vector<framework::JsonObject> data;

	for (const auto& value : result)
	{
		framework::JsonObject object;

		object["id"] = value.at("id").get<int64_t>();
		object["name"] = value.at("name").get<std::string>();
		object["amount"] = value.at("amount").get<int64_t>();

		data.push_back(std::move(object));
	}

	response.setBody(framework::JsonBuilder().append("data", std::move(data)));
}

void CRUDExecutor::doPost(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.getOrCreateDatabase("test_database").getOrCreateTable
	(
		"test_table",
		"CREATE TABLE IF NOT EXISTS test_table ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name VARCHAR(255) NOT NULL, "
		"amount INTEGER NOT NULL)"
	);
}

void CRUDExecutor::doPut(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Database database = request.getDatabase("test_database");
	framework::Table table = database.getTable("test_table");
	std::mt19937_64 random(std::time(nullptr));

	table.execute
	(
		"INSERT INTO test_table (name, amount) VALUES(?, ?)",
		framework::utility::database::makeSQLValues("glue", -1)
	);

	for (size_t i = 0; i < 10; i++)
	{
		table.execute
		(
			"INSERT INTO test_table (name, amount) VALUES(?, ?)",
			framework::utility::database::makeSQLValues("glue", random() % 200)
		);
	}
}

void CRUDExecutor::doPatch(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Database database = request.getDatabase("test_database");
	framework::Table table = database.getTable("test_table");

	table.execute
	(
		"UPDATE test_table "
		"SET name = ? "
		"WHERE amount = ?",
		framework::utility::database::makeSQLValues("empty", -1)
	);
	framework::SqlResult result = table.execute
	(
		"SELECT * FROM test_table WHERE name = ?",
		framework::utility::database::makeSQLValues("empty")
	);

	std::vector<framework::JsonObject> data;

	for (const auto& value : result)
	{
		framework::JsonObject object;

		object["id"] = value.at("id").get<int64_t>();
		object["name"] = value.at("name").get<std::string>();
		object["amount"] = value.at("amount").get<int64_t>();

		data.push_back(std::move(object));
	}

	response.setBody(framework::JsonBuilder().append("data", std::move(data)));
}

void CRUDExecutor::doDelete(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.getDatabase("test_database").getTable("test_table").execute("DROP TABLE test_table", {});
}

DEFINE_EXECUTOR(CRUDExecutor)
