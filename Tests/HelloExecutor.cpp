#include "HelloExecutor.h"

#include "JSONBuilder.h"

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	json::JSONBuilder builder(CP_UTF8);

	builder["message"] = "Hello, World!";
	builder["randomNumber"] = rand();

	response.addBody(builder);
}

DECLARE_EXECUTOR(HelloExecutor)
