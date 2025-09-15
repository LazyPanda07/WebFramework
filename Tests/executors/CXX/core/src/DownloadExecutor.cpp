#include "DownloadExecutor.h"

void DownloadExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.streamFile("index.html", response, "index.html");
}

DEFINE_EXECUTOR(DownloadExecutor)
