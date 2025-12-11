#include <Executors/Executor.h>

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
	JsonBuilder builder = NULL;
	struct timespec duration = { 10, 0 };

	createJsonBuilder(&builder);
	appendJsonBuilderInteger(builder, "id", getProcessId());

	thrd_sleep(&duration, NULL);

	setJsonBody(response, builder);

	deleteWebFrameworkJsonBuilder(builder);
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
