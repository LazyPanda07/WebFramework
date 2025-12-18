#include "DynamicResources.h"

void DynamicResources::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	std::string fileData = request.getFile("page.md");

	response.setBody(request.processStaticFile(fileData, ".md"));
}

void DynamicResources::doPost(framework::HttpRequest& request, framework::HttpResponse& response)
{
	std::string fileData = request.getFile("print.wfdp");
	std::unordered_map<std::string, std::string> variables;

	variables.try_emplace("data", request.getJson().get<std::string>("data"));

	response.setBody(request.processDynamicFile(fileData, variables));
}

DEFINE_EXECUTOR(DynamicResources)
