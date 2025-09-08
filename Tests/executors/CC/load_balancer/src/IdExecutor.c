#include <APIExecutors/BaseExecutor.h>

#include <threads.h>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

static int64_t getProcessId();

DEFINE_DEFAULT_EXECUTOR(IdExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(IdExecutor, GET_METHOD, request, response)
{
	JSONBuilder builder = NULL;
	struct timespec duration = { 10, 0 };

	createJSONBuilder(&builder);
	appendJSONBuilderInteger(builder, "id", getProcessId());

	thrd_sleep(&duration, NULL);

	setJSONBody(response, builder);

	deleteWebFrameworkJSONBuilder(builder);
}

int64_t getProcessId()
{
#ifdef __LINUX__
	return (int64_t)getpid();
#else
	return (int64_t)GetCurrentProcessId();
#endif
}

DEFINE_INITIALIZE_WEB_FRAMEWORK();
