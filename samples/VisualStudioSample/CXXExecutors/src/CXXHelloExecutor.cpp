#include "CXXHelloExecutor.h"

void CXXHelloExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	response.setBody(framework::JsonBuilder().append("message", "Hello from C++"));
}

DEFINE_EXECUTOR(CXXHelloExecutor)
