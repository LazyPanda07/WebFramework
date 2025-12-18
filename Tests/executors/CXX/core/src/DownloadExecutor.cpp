#include "DownloadExecutor.h"

void DownloadExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.streamFile("index.html", response, "index.html");
}

DEFINE_EXECUTOR(DownloadExecutor)
