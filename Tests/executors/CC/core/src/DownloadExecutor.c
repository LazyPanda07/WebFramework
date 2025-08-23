#include <APIExecutors/BaseExecutor.h>

#include <Utility/WebFrameworkUtility.h>

DECLARE_DEFAULT_EXECUTOR(DownloadExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR);

DECLARE_EXECUTOR_METHOD(DownloadExecutor, GET_METHOD, request, response)
{
	streamFile(request, "index.html", response, "index.html", DEFAULT_CHUNK_SIZE);
}
