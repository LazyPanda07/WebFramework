#include "Index.h"

using namespace std;

void Index::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
{
	first = get<string>(settings.initParameters->data[0].second);

	second = get<int64_t>(settings.initParameters->data[1].second);

	third = get<double>(settings.initParameters->data[2].second);

	fourth = &get<vector<json::utility::objectSmartPointer<json::utility::jsonObject>>>(settings.initParameters->data[3].second);
}

void Index::doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response)
{
	// if (!request.isDynamicFunctionRegistered("br"))
	// {
	// 	request.registerDynamicFunction("br", [](const vector<string>& values) -> string { return values[0] + "<br>"; });
	// }
	// 
	// request.sendAssetFile("/index.wfdp", response);

	string body;

	body.append(first).append("<br>").
		append(to_string(second)).append("<br>").
		append(to_string(third)).append("<br>");

	for (const auto& i : *fourth)
	{
		body.append(to_string(get<int64_t>(i->data.front().second))).append(" ");
	}

	response.addBody
	(
		move(body)
	);
}
