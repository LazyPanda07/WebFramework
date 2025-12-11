#include "DynamicResources.h"

void DynamicResources::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::string fileData = request.getFile("page.md");

	response.setBody(request.processStaticFile(fileData, ".md"));
}

void DynamicResources::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::string fileData = request.getFile("print.wfdp");
	std::unordered_map<std::string, std::string> variables;

	variables.try_emplace("data", request.getJson().get<std::string>("data"));

	response.setBody(request.processWFDPFile(fileData, variables));
}

DEFINE_EXECUTOR(DynamicResources)
