#include "UserAgentExecutor.h"

void UserAgentExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody("ok");
}

DEFINE_EXECUTOR(UserAgentExecutor)
