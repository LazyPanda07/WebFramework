#include "UserAgentExecutor.h"

void UserAgentExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody("ok");
}

DECLARE_EXECUTOR(UserAgentExecutor)
