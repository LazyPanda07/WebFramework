#include "HelloExecutor.h"

#include "JSONBuilder.h"

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody(json::JSONBuilder(CP_UTF8).appendString("message", "Hello, World!"));
}

DECLARE_EXECUTOR(HelloExecutor)
