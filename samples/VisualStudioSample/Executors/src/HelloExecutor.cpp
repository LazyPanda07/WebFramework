#include "HelloExecutor.h"

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody(framework::JSONBuilder().append("message", "Hello, World!"));
}

DEFINE_EXECUTOR(HelloExecutor)
