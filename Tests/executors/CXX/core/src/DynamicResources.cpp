#include "DynamicResources.h"

void DynamicResources::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	std::string fileData = request.getFile("page.md");

	response.setBody(request.processStaticFile(fileData, ".md"));
}

void DynamicResources::doPost(framework::HttpRequest& request, framework::HttpResponse& response)
{
	std::string fileData = request.getFile("print.wfdp");
	framework::JsonObject arguments;

	arguments["@print"] = request.getJson().getParsedData();

	response.setBody(request.processDynamicFile(fileData, arguments));
}

DEFINE_EXECUTOR(DynamicResources)
