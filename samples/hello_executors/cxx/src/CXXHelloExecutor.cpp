#include "CXXHelloExecutor.h"

namespace executors
{
	void CXXHelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		response.setBody(framework::JsonBuilder().append("message", "Hello, World!"));
	}

	DEFINE_EXECUTOR(CXXHelloExecutor);
}
