#include "CRUDExecutor.h"

#include <random>
#include <ctime>

#include "DatabaseModel.h"

void CRUDExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::shared_ptr<DatabaseModel> model = request.getModel<DatabaseModel>();
	framework::sqlite::utility::SQLiteResult result = model->selectByField("name", "glue");
	std::vector<json::utility::jsonObject> data;

	for (const auto& value : result)
	{
		json::utility::jsonObject object;

		for (const auto& [key, printValue] : value)
		{
			std::cout << key << ' ' << printValue << std::endl;
		}

		object["id"] = value.at("id");
		object["name"] = value.at("name");
		object["amount"] = value.at("amount");

		json::utility::appendArray(object, data);
	}

	response.addBody(json::JSONBuilder(CP_UTF8).appendArray("data", std::move(data)));
}

void CRUDExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.createModel<DatabaseModel>()->createTable
	(
		{
			{ "id", "INTEGER PRIMARY KEY AUTOINCREMENT" },
			{ "name", "VARCHAR(255) NOT NULL" },
			{ "amount", "INTEGER NOT NULL" }
		}
	);
}

void CRUDExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::shared_ptr<DatabaseModel> model = request.getModel<DatabaseModel>();
	std::mt19937_64 random(std::time(nullptr));

	model->insert
	(
		{
			{ "name", "glue" },
			{ "amount", "-1" }
		}
	);

	for (size_t i = 0; i < 10; i++)
	{
		model->insert
		(
			{
				{ "name", "glue" },
				{ "amount", std::to_string(random() % 200) }
			}
		);
	}
}

void CRUDExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::shared_ptr<DatabaseModel> model = request.getModel<DatabaseModel>();
	model->update
	(
		{
			{ "name", "empty" }
		},
		"amount",
		"-1"
	);

	framework::sqlite::utility::SQLiteResult result = model->selectByField("name", "empty");
	std::vector<json::utility::jsonObject> data;

	for (const auto& value : result)
	{
		json::utility::jsonObject object;

		object["id"] = value.at("id");
		object["name"] = value.at("name");
		object["amount"] = value.at("amount");

		json::utility::appendArray(object, data);
	}

	response.addBody(json::JSONBuilder(CP_UTF8).appendArray("data", std::move(data)));
}

void CRUDExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.getModel<DatabaseModel>()->dropTable();
}

DECLARE_EXECUTOR(CRUDExecutor)
