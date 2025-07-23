#include "MultiUserExecutor.h"

#include "UUID.h"

MultiUserExecutor::MultiUserExecutor() :
    userId(utility::uuid::generateUUID())
{
	
}

void MultiUserExecutor::doGet(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response)
{
	framework::Table table = request.getTable("test_database", "multi_user");

	framework::SQLResult result = table.execute
	(
		"SELECT * FROM multi_user WHERE user_id = ?",
		{ framework::SQLValue(userId) }
	);
	std::vector<json::utility::jsonObject> data;

	for (const auto& value : result)
	{
		json::utility::jsonObject object;

		object.setInt("id", value.at("id").get<int64_t>());
		object.setString("user_id", value.at("user_id").get<std::string>());
		object.setString("data", value.at("data").get<std::string>());

		json::utility::appendArray(object, data);
	}

	response.setBody(json::JSONBuilder(CP_UTF8).appendArray("data", std::move(data)));
}

void MultiUserExecutor::doPost(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response)
{
	request.getOrCreateDatabase("test_database").getOrCreateTable("multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)");
}

void MultiUserExecutor::doPut(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response)
{
	framework::Table table = request.getTable("test_database", "multi_user");

	table.execute
	(
		"INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
		{ framework::SQLValue(userId), framework::SQLValue(request.getJSON().getString("data")) }
	);
}

DECLARE_EXECUTOR(MultiUserExecutor)
