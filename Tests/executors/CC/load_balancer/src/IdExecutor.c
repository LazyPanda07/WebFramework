#include <executors/executor.h>

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
	json_builder_t builder = NULL;
	struct timespec duration = { 10, 0 };

	wf_create_json_builder(&builder);
	wf_append_json_builder_integer(builder, "id", getProcessId());

	thrd_sleep(&duration, NULL);

	wf_set_json_body(response, builder);

	wf_delete_web_framework_json_builder(builder);
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
