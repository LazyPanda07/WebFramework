#include <Executors/BaseExecutor.h>

#include <Utility/WebFrameworkUtility.h>

DEFINE_DEFAULT_EXECUTOR(DownloadExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(DownloadExecutor, GET_METHOD, request, response)
{
	streamFile(request, "index.html", response, "index.html", DEFAULT_CHUNK_SIZE);
}
