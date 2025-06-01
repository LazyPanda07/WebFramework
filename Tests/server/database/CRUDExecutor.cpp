#include "CRUDExecutor.h"

#include <random>
#include <ctime>

void CRUDExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	framework::Database model = request.getDatabase("test_database");
	framework::Table table = model.getTable("test_table");
	framework::SQLResult result = table.execute("SELECT * FROM test_table WHERE name = ?", { framework::SQLValue("glue") });
	std::vector<json::utility::jsonObject> data;

	for (const auto& value : result)
	{
		json::utility::jsonObject object;

		object.setInt("id", value.at("id").get<int64_t>());
		object.setString("name", value.at("name").get<std::string>());
		object.setInt("amount", value.at("amount").get<int64_t>());

		json::utility::appendArray(object, data);
	}

	response.setBody(json::JSONBuilder(CP_UTF8).appendArray("data", std::move(data)));
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
		{ framework::SQLValue("glue"), framework::SQLValue(-1) }
	);

	for (size_t i = 0; i < 10; i++)
	{
		table.execute
		(
			"INSERT INTO test_table (name, amount) VALUES(?, ?)",
			{ framework::SQLValue("glue"), framework::SQLValue(std::to_string(random() % 200)) }
		);
	}
}

void CRUDExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	framework::Database database = request.getDatabase("test_database");
	framework::Table table = database.getTable("test_table");
	framework::SQLResult result = table.execute
	(
		"UPDATE test_table "
		"SET name = ? "
		"WHERE amount = ?",
		{ framework::SQLValue("empty"), framework::SQLValue(-1) }
	);

	std::vector<json::utility::jsonObject> data;

	for (const auto& value : result)
	{
		json::utility::jsonObject object;

		object.setInt("id", value.at("id").get<int64_t>());
		object.setString("name", value.at("name").get<std::string>());
		object.setInt("amount", value.at("amount").get<int64_t>());

		json::utility::appendArray(object, data);
	}

	response.setBody(json::JSONBuilder(CP_UTF8).appendArray("data", std::move(data)));
}

void CRUDExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.getDatabase("test_database").getTable("test_table").execute("DROP TABLE test_table", {});
}

DECLARE_EXECUTOR(CRUDExecutor)
