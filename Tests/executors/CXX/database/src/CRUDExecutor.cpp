#include "CRUDExecutor.h"

#include <random>
#include <ctime>

#include <Utility/WebFrameworkUtility.hpp>

void CRUDExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	framework::Database database = request.getDatabase("test_database");
	framework::Table table = database.getTable("test_table");
	framework::SQLResult result = table.execute("SELECT * FROM test_table WHERE name = ?", framework::utility::database::makeSQLValues("glue"));
	std::vector<framework::JsonObject> data;

	for (const auto& value : result)
	{
		framework::JsonObject object;

		object.setValue("id", value.at("id").get<int64_t>());
		object.setValue("name", value.at("name").get<std::string>());
		object.setValue("amount", value.at("amount").get<int64_t>());

		framework::utility::appendArray(data, object);
	}

	response.setBody(framework::JsonBuilder().append("data", std::move(data)));
}

void CRUDExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
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

void CRUDExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
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

void CRUDExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
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
	framework::SQLResult result = table.execute
	(
		"SELECT * FROM test_table WHERE name = ?",
		framework::utility::database::makeSQLValues("empty")
	);

	std::vector<framework::JsonObject> data;

	for (const auto& value : result)
	{
		framework::JsonObject object;

		object.setValue("id", value.at("id").get<int64_t>());
		object.setValue("name", value.at("name").get<std::string>());
		object.setValue("amount", value.at("amount").get<int64_t>());

		framework::utility::appendArray(data, object);
	}

	response.setBody(framework::JsonBuilder().append("data", std::move(data)));
}

void CRUDExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.getDatabase("test_database").getTable("test_table").execute("DROP TABLE test_table", {});
}

DEFINE_EXECUTOR(CRUDExecutor)
