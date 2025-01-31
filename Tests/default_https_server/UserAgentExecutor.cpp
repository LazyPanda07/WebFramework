#include "UserAgentExecutor.h"

void UserAgentExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.addBody("ok");
}

DECLARE_EXECUTOR(UserAgentExecutor)
