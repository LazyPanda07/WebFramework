#include "AssetsExecutor.h"

#include <format>

class CustomFunctionClass : public framework::DynamicFunction
{
public:
	std::string operator ()(const framework::JsonObject& arguments) override
	{
		const std::vector<framework::JsonObject>& data = arguments["@customFunction"]["data"].get<std::vector<framework::JsonObject>>();

		return std::format("Data: {} {} {}", data[0].get<int64_t>(), data[1].get<int64_t>(), data[2].get<int64_t>());
	}
};

void AssetsExecutor::init(const framework::utility::ExecutorSettings& settings)
{
	settings.registerDynamicFunctionClass<CustomFunctionClass>("customFunction");
}

void AssetsExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	const std::unordered_map<std::string, std::string>& queryParameters = request.getQueryParameters();
	framework::JsonObject arguments;

	if (auto it = queryParameters.find("data"); it != queryParameters.end())
	{
		arguments["@print"]["data"] = it->second;
	}
	else if (queryParameters.contains("first"))
	{
		framework::JsonObject data = arguments["@customFunction"]["data"];

		data.emplace_back(std::stoll(queryParameters.at("first")));
		data.emplace_back(std::stoll(queryParameters.at("second")));
		data.emplace_back(std::stoll(queryParameters.at("third")));
	}

	request.sendDynamicFile
	(
		std::format("{}.wfdp", request.getJson().get<std::string>("fileName")),
		response,
		arguments
	);
}

void AssetsExecutor::doDelete(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.unregisterDynamicFunction("customFunction");
}

DEFINE_EXECUTOR(AssetsExecutor)
