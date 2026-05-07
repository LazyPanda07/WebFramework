#include "DownloadExecutor.h"

void DownloadExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	const std::string& fileName = request.getJson().get<std::string>("fileName");

	request.streamFile(fileName, response, fileName);
}

DEFINE_EXECUTOR(DownloadExecutor)
