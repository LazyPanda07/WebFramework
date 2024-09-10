#include "HelloExecutor.h"

#include "JSONBuilder.h"

namespace executors
{
	void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		response.addBody(json::JSONBuilder(CP_UTF8).appendString("message", "Hello, World!"));
	}

	DECLARE_EXECUTOR(HelloExecutor);
}
