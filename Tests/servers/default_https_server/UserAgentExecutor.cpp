#include "UserAgentExecutor.h"

void UserAgentExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	response.setBody("ok");
}

DEFINE_EXECUTOR(UserAgentExecutor)
