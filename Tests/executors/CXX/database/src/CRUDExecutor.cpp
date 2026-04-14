#include "CRUDExecutor.h"

#include <random>
#include <ctime>

#include <Utility/WebFrameworkUtility.hpp>

struct TableData
{
public:
	static TableData create(const framework::SqlResult::Row& row);

public:
	int64_t id;
	std::string name;
	int64_t amount;
};

void CRUDExecutor::init(const framework::utility::ExecutorSettings& settings)
{
	settings.getOrCreateDatabase("test_database").getOrCreateTable
	(
		"test_table",
		"CREATE TABLE IF NOT EXISTS test_table ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name VARCHAR(255) NOT NULL, "
		"amount INTEGER NOT NULL)"
	);
}

void CRUDExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Database database = request.getDatabase("test_database");
	framework::Table table = database.getTable("test_table");
	std::vector<TableData> result = table.execute<TableData>("SELECT * FROM test_table WHERE name = ?", framework::utility::database::makeSqlValues("glue"));
	std::vector<framework::JsonObject> data;

	for (const TableData& value : result)
	{
		framework::JsonObject object;

		object["id"] = value.id;
		object["name"] = value.name;
		object["amount"] = value.amount;

		data.push_back(std::move(object));
	}

	response.setBody(framework::JsonBuilder().append("data", std::move(data)));
}

void CRUDExecutor::doPut(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Database database = request.getDatabase("test_database");
	framework::Table table = database.getTable("test_table");
	std::mt19937_64 random(std::time(nullptr));

	table.execute
	(
		"INSERT INTO test_table (name, amount) VALUES(?, ?)",
		framework::utility::database::makeSqlValues("glue", -1)
	);

	for (size_t i = 0; i < 10; i++)
	{
		table.execute
		(
			"INSERT INTO test_table (name, amount) VALUES(?, ?)",
			framework::utility::database::makeSqlValues("glue", random() % 200)
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
		framework::utility::database::makeSqlValues("empty", -1)
	);
	std::vector<TableData> result = table.execute<TableData>
	(
		"SELECT * FROM test_table WHERE name = ?",
		framework::utility::database::makeSqlValues("empty")
	);

	std::vector<framework::JsonObject> data;

	for (const TableData& value : result)
	{
		framework::JsonObject object;

		object["id"] = value.id;
		object["name"] = value.name;
		object["amount"] = value.amount;

		data.push_back(std::move(object));
	}

	response.setBody(framework::JsonBuilder().append("data", std::move(data)));
}

void CRUDExecutor::doDelete(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.getDatabase("test_database").getTable("test_table").execute("DROP TABLE test_table", {});
}

DEFINE_EXECUTOR(CRUDExecutor)

TableData TableData::create(const framework::SqlResult::Row& row)
{
	TableData result =
	{
		.id = row.at("id").get<int64_t>(),
		.name = row.at("name").get<std::string>(),
		.amount = row.at("amount").get<int64_t>()
	};

	return result;
}
