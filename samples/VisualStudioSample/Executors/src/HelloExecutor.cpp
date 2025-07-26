#include "HelloExecutor.h"

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody(framework::JSONBuilder().append("message", "Hello, World!"));
}

DECLARE_EXECUTOR(HelloExecutor)
