#include "HelloExecutor.h"

#include <JSONBuilder.h>

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	try
	{
		json::JSONBuilder builder(CP_UTF8);

		builder["message"] = "Hello, World!";
		builder["randomNumber"] = rand();

		response.addBody(builder);
	}
	catch (const std::out_of_range&)
	{
		exit(-3);
	}
}

DECLARE_EXECUTOR(HelloExecutor)
