#include "DownloadExecutor.h"

void DownloadExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.streamFile("index.html", response, "index.html");
}

DECLARE_EXECUTOR(DownloadExecutor)
