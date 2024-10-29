#include "MultiUserExecutor.h"

#include "MultiUserDatabaseModel.h"
#include "UUID.h"

MultiUserExecutor::MultiUserExecutor() :
    userId(utility::generateUUID())
{
	
}

void MultiUserExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::shared_ptr<MultiUserDatabaseModel> model = request.getModel<MultiUserDatabaseModel>();
	framework::sqlite::utility::SQLiteResult result = model->selectByField("user_id", userId);
	std::vector<json::utility::jsonObject> data;

	for (const auto& value : result)
	{
		json::utility::jsonObject object;

		object.setString("id", value.at("id"));
		object.setString("user_id", value.at("user_id"));
		object.setString("data", value.at("data"));

		json::utility::appendArray(object, data);
	}

	response.addBody(json::JSONBuilder(CP_UTF8).appendArray("data", std::move(data)));
}

void MultiUserExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.createModel<MultiUserDatabaseModel>();
}

void MultiUserExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::shared_ptr<MultiUserDatabaseModel> model = request.getModel<MultiUserDatabaseModel>();

	model->insert
    (
        {
            { "user_id", userId },
            { "data", request.getJSON().getString("data") }
        }
    );
}

DECLARE_EXECUTOR(MultiUserExecutor)
