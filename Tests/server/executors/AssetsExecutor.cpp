#include "AssetsExecutor.h"

#include <format>
#include <numeric>

static std::string customFunction(const std::vector<std::string>& args)
{
	using namespace std::string_literals;

	return std::accumulate(args.begin(), args.end(), "Data: "s, [](const std::string& left, const std::string& right) { return std::format("{} {}", left, right); });
}

void AssetsExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.sendDynamicFile
	(
		std::format("{}.wfdp", request.getJSON().getString("fileName")),
		response,
		request.getKeyValueParameters()
	);
}

void AssetsExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.registerDynamicFunction("customFunction", customFunction);
}

void AssetsExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.unregisterDynamicFunction("customFunction");
}

DECLARE_EXECUTOR(AssetsExecutor)
