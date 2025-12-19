#include "HelloExecutor.h"

namespace executors
{
	void HelloExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
	{
		response.setBody(framework::JsonBuilder().append("message", "Hello, World!"));
	}

	DEFINE_EXECUTOR(HelloExecutor);
}
