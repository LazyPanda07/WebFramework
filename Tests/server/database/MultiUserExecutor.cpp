#include "MultiUserExecutor.h"

#include <mutex>

#include "MultiUserDatabaseModel.h"

static size_t getId()
{
    static std::atomic_size_t nextIndex = 0;

    return nextIndex++;
}

MultiUserExecutor::MultiUserExecutor() :
    userId(getId())
{
    
}

void MultiUserExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::shared_ptr<MultiUserDatabaseModel> model = request.getModel<MultiUserDatabaseModel>();
	framework::sqlite::utility::SQLiteResult result = model->selectByField("user_id", std::to_string(userId));
	std::vector<json::utility::jsonObject> data;

	for (const auto& value : result)
	{
		json::utility::jsonObject object;

		object.setInt("id", std::stoi(value.at("id")));
        object.setInt("user_id", std::stoi(value.at("user_id")));
		object.setString("data", value.at("data"));

		json::utility::appendArray(object, data);
	}

	response.addBody(json::JSONBuilder(CP_UTF8).appendArray("data", std::move(data)));
}

void MultiUserExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.createModel<MultiUserDatabaseModel>()->createTable
	(
		{
			{ "id", "INTEGER PRIMARY KEY AUTOINCREMENT" },
			{ "user_id", "INTEGER NOT NULL" },
			{ "data", "VARCHAR(255) NOT NULL" }
		}
	);
}

void MultiUserExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::shared_ptr<MultiUserDatabaseModel> model = request.getModel<MultiUserDatabaseModel>();

	model->insert
    (
        {
            { "user_id", std::to_string(userId) },
            { "data", request.getJSON().getString("data") }
        }
    );
}

DECLARE_EXECUTOR(MultiUserExecutor)
