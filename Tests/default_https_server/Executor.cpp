#include "Executor.h"

#include "JSONBuilder.h"

void Executor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.addBody(json::JSONBuilder(CP_UTF8).appendString("data", "data"));
}

DECLARE_EXECUTOR(Executor)
