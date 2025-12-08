#include "MultiUserExecutor.h"

#include <Utility/WebFrameworkUtility.hpp>

MultiUserExecutor::MultiUserExecutor() :
    userId(framework::utility::uuid::generateUUID())
{
	
}

void MultiUserExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	framework::Table table = request.getTable("test_database", "multi_user");

	framework::SQLResult result = table.execute
	(
		"SELECT * FROM multi_user WHERE user_id = ?",
		framework::utility::database::makeSQLValues(userId)
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

void MultiUserExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.getOrCreateDatabase("test_database").getOrCreateTable("multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)");
}

void MultiUserExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	framework::Table table = request.getTable("test_database", "multi_user");

	table.execute
	(
		"INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
		framework::utility::database::makeSQLValues(userId, request.getJSON().get<std::string>("data"))
	);
}

DEFINE_EXECUTOR(MultiUserExecutor)
