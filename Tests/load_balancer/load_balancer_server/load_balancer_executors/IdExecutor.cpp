#include "IdExecutor.h"

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

static int64_t getProcessId();

void IdExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody
	(
		json::JSONBuilder(CP_UTF8).appendInt("id", getProcessId())
	);
}

int64_t getProcessId()
{
#ifdef __LINUX__
	return static_cast<int64_t>(getpid());
#else
	return static_cast<int64_t>(GetCurrentProcessId());
#endif
}

DECLARE_EXECUTOR(IdExecutor)
