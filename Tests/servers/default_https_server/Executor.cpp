#include "Executor.h"

void Executor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody(framework::JSONBuilder().append("data", "data"));
}

DECLARE_EXECUTOR(Executor)
