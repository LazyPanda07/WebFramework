#include "UserAgentExecutor.h"

void UserAgentExecutor::doGet(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response)
{
	response.setBody("ok");
}

DECLARE_EXECUTOR(UserAgentExecutor)
