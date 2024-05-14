#include "AssetsExecutor.h"

#include <format>
#include <numeric>

static std::string customFunction(const std::vector<std::string>& args)
{
	return std::format("Data: {} {} {}", args[0], args[1], args[2]);
}

void AssetsExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	for (const auto& [key, value] : request.getKeyValueParameters())
	{
		std::cout << key << ' ' << value << std::endl;
	}

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
