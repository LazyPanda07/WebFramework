#include "AssetsExecutor.h"

#include <format>

class CustomFunctionClass : public framework::DynamicFunction
{
public:
	std::string operator ()(const std::span<std::string_view>& arguments) override
	{
		return std::format("Data: {} {} {}", arguments[0], arguments[1], arguments[2]);
	}
};

static const char* customFunction(const char** args, size_t agumentsNumber)
{
	std::string temp = std::format("Data: {} {} {}", args[0], args[1], args[2]);
	char* result = new char[temp.size() + 1];
	
	result[temp.size()] = '\0';

	temp.copy(result, temp.size());
	
	return result;
}

void AssetsExecutor::init(const framework::utility::ExecutorSettings& settings)
{
	settings.registerDynamicFunctionClass<CustomFunctionClass>("customFunction");
}

void AssetsExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.sendDynamicFile
	(
		std::format("{}.wfdp", request.getJson().get<std::string>("fileName")),
		response,
		request.getQueryParameters()
	);
}

void AssetsExecutor::doDelete(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.unregisterDynamicFunction("customFunction");
}

DEFINE_EXECUTOR(AssetsExecutor)
