#include "IdExecutor.h"

#include <thread>
#include <chrono>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

static int64_t getProcessId();

void IdExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	using namespace std::chrono_literals;

	std::this_thread::sleep_for(10s);

	response.setBody
	(
		framework::JsonBuilder().append("id", getProcessId())
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

DEFINE_EXECUTOR(IdExecutor)
