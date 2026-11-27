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
	settings.getDatabase<framework::PostgresDatabase>("test_database").getOrCreateTable
	(
		"multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY, user_id TEXT NOT NULL, data TEXT NOT NULL)"
	);
#endif

	settings.getOrCreateDatabase<framework::SqliteDatabase>("test_database").getOrCreateTable("multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)");
}

void MultiUserExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Table table = getTable("test_database", "multi_user", request);
	std::string databaseImplementation = request.getRouteParameter<std::string>("databaseImplementation");
	std::string sqlRequest;

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		sqlRequest = "SELECT * FROM multi_user WHERE user_id = ?";
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		sqlRequest = "SELECT * FROM multi_user WHERE user_id = $1";
	}

	framework::SqlResult result = table.execute(sqlRequest, framework::utility::database::makeSqlValues(userId));

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
	std::string databaseImplementation = request.getRouteParameter<std::string>("databaseImplementation");

	if (databaseImplementation == framework::SqliteDatabase::databaseImplementationName)
	{
		table.execute
		(
			"INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
			framework::utility::database::makeSqlValues(userId, request.getJson().get<std::string>("data"))
		);
	}
	else if (databaseImplementation == framework::PostgresDatabase::databaseImplementationName)
	{
		table.execute
		(
			"INSERT INTO multi_user (user_id, data) VALUES($1, $2)",
			framework::utility::database::makeSqlValues(userId, request.getJson().get<std::string>("data"))
		);
	}
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
