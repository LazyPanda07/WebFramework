#include <executors/executor.h>

#include <utility/web_framework_utility.h>

DEFINE_DEFAULT_EXECUTOR(DownloadExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(DownloadExecutor, GET_METHOD, request, response)
{
	wf_stream_file(request, "index.html", response, "index.html", DEFAULT_CHUNK_SIZE);
}
