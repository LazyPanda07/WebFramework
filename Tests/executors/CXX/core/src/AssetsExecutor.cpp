#include "AssetsExecutor.h"

#include <format>
#include <numeric>

static const char* customFunction(const char** args, size_t agumentsNumber)
{
	std::string temp = std::format("Data: {} {} {}", args[0], args[1], args[2]);
	char* result = new char[temp.size() + 1];
	
	result[temp.size()] = '\0';

	temp.copy(result, temp.size());
	
	return result;
}

void AssetsExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.sendWFDPFile
	(
		std::format("{}.wfdp", request.getJSON().get<std::string>("fileName")),
		response,
		request.getQueryParameters()
	);
}

void AssetsExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.registerWFDPFunction("customFunction", customFunction, [](char* result) { delete[] result; });
}

void AssetsExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.unregisterWFDPFunction("customFunction");
}

DEFINE_EXECUTOR(AssetsExecutor)
