#include "CXXHelloExecutor.h"

void CXXHelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody(framework::JSONBuilder().append("message", "Hello, World!"));
}

DEFINE_EXECUTOR(CXXHelloExecutor)
