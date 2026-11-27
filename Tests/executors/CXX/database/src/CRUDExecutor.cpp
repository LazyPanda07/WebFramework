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

static framework::Database getDatabase(std::string_view databaseName, framework::HttpRequest& request);

void CRUDExecutor::init(const framework::utility::ExecutorSettings& settings)
{
#ifndef WITHOUT_POSTGRES_TESTS
	std::string host = "127.0.0.1";
	std::string password = "root";

	if (char* overrideHost = std::getenv("POSTGRES_HOST"))
	{
		host = overrideHost;
	}

	if (char* overridePassword = std::getenv("POSTGRES_PASSWORD"))
	{
		password = overridePassword;
	}

	settings.constructDatabase<framework::PostgresDatabase>("test_database", std::format("host={} port=5432 dbname=test_database user=postgres password={}", host, password)).getOrCreateTable
	(
		"test_table",
		"CREATE TABLE IF NOT EXISTS test_table ("
		"id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY, "
		"name VARCHAR(255) NOT NULL, "
		"amount INTEGER NOT NULL)"
	);
#endif

	settings.getOrCreateDatabase<framework::SqliteDatabase>("test_database").getOrCreateTable
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
	framework::Database database = getDatabase("test_database", request);
	framework::Table table = database.getTable("test_table");
	std::string databaseImplementation = request.getRouteParameter<std::string>("databaseImplementation");
	std::vector<TableData> result;
	std::vector<framework::JsonObject> data;

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		result = table.execute<TableData>("SELECT * FROM test_table WHERE name = ?", framework::utility::database::makeSqlValues("glue"));
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		result = table.execute<TableData>("SELECT * FROM test_table WHERE name = $1", framework::utility::database::makeSqlValues("glue"));
	}

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
	framework::Database database = getDatabase("test_database", request);
	framework::Table table = database.getTable("test_table");
	std::mt19937_64 random(std::time(nullptr));
	std::string databaseImplementation = request.getRouteParameter<std::string>("databaseImplementation");

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		table.execute
		(
			"INSERT INTO test_table (name, amount) VALUES(?, ?)",
			framework::utility::database::makeSqlValues("glue", -1)
		);
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		table.execute
		(
			"INSERT INTO test_table (name, amount) VALUES($1, $2)",
			framework::utility::database::makeSqlValues("glue", -1)
		);
	}

	for (size_t i = 0; i < 10; i++)
	{
		if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
		{
			table.execute
			(
				"INSERT INTO test_table (name, amount) VALUES(?, ?)",
				framework::utility::database::makeSqlValues("glue", random() % 200)
			);
		}
		else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
		{
			table.execute
			(
				"INSERT INTO test_table (name, amount) VALUES($1, $2)",
				framework::utility::database::makeSqlValues("glue", random() % 200)
			);
		}
	}
}

void CRUDExecutor::doPatch(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Database database = getDatabase("test_database", request);
	framework::Table table = database.getTable("test_table");
	std::string databaseImplementation = request.getRouteParameter<std::string>("databaseImplementation");

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		table.execute
		(
			"UPDATE test_table "
			"SET name = ? "
			"WHERE amount = ?",
			framework::utility::database::makeSqlValues("empty", -1)
		);
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		table.execute
		(
			"UPDATE test_table "
			"SET name = $1 "
			"WHERE amount = $2",
			framework::utility::database::makeSqlValues("empty", -1)
		);
	}

	std::vector<TableData> result;

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		result = table.execute<TableData>
			(
				"SELECT * FROM test_table WHERE name = ?",
				framework::utility::database::makeSqlValues("empty")
			);
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		result = table.execute<TableData>
			(
				"SELECT * FROM test_table WHERE name = $1",
				framework::utility::database::makeSqlValues("empty")
			);
	}

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
	getDatabase("test_database", request).getTable("test_table").execute("DROP TABLE test_table", {});
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

framework::Database getDatabase(std::string_view databaseName, framework::HttpRequest& request)
{
	std::string databaseImplementation = request.getRouteParameter<std::string>("databaseImplementation");

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		return request.getDatabase<framework::SqliteDatabase>(databaseName);
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		return request.getDatabase<framework::PostgresDatabase>(databaseName);
	}

	request.throwException(std::format("Wrong databaseImplementation: {}", databaseImplementation), framework::ResponseCodes::badRequest);

	return request.getDatabase(databaseName);
}
