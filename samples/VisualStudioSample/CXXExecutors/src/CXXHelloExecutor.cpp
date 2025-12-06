#include "CXXHelloExecutor.h"

void CXXHelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody(framework::JsonBuilder().append("message", "Hello, World from C++!"));
}

DEFINE_EXECUTOR(CXXHelloExecutor)
