#include "MultiUserExecutor.h"

#include <Utility/WebFrameworkUtility.hpp>

static framework::Table getTable(std::string_view databaseName, std::string_view tableName, framework::HttpRequest& request);

MultiUserExecutor::MultiUserExecutor() :
    userId(framework::utility::uuid::generateUuid())
{
	
}

void MultiUserExecutor::init(const framework::utility::ExecutorSettings& settings)
{
#ifndef WITHOUT_POSTGRES_TESTS
	std::string host = "127.0.0.1";
	std::string password = "secret";

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
		"multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY, user_id TEXT NOT NULL, data TEXT NOT NULL)"
	);
#endif

	settings.getOrCreateDatabase<framework::SqliteDatabase>("test_database").getOrCreateTable("multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)");
}

void MultiUserExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Table table = getTable("test_database", "multi_user", request);

	framework::SqlResult result = table.execute
	(
		"SELECT * FROM multi_user WHERE user_id = ?",
		framework::utility::database::makeSqlValues(userId)
	);
	std::vector<framework::JsonObject> data;

	for (const auto& value : result)
	{
		framework::JsonObject object;

		object["id"] = value.at("id").get<int64_t>();
		object["user_id"] = value.at("user_id").get<std::string>();
		object["data"] = value.at("data").get<std::string>();

		data.push_back(std::move(object));
	}

	response.setBody(framework::JsonBuilder().append("data", std::move(data)));
}

void MultiUserExecutor::doPut(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Table table = getTable("test_database", "multi_user", request);

	table.execute
	(
		"INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
		framework::utility::database::makeSqlValues(userId, request.getJson().get<std::string>("data"))
	);
}

DEFINE_EXECUTOR(MultiUserExecutor)

framework::Table getTable(std::string_view databaseName, std::string_view tableName, framework::HttpRequest& request)
{
	std::string databaseImplementation = request.getRouteParameter<std::string>("databaseImplementation");

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		return request.getTable<framework::SqliteDatabase>(databaseName, tableName);
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		return request.getTable<framework::PostgresDatabase>(databaseName, tableName);
	}

	request.throwException(std::format("Wrong databaseImplementation: {}", databaseImplementation), framework::ResponseCodes::badRequest);

	return request.getTable(databaseName, tableName);
}
