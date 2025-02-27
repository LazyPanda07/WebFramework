#include "UploadOctetStreamExecutor.h"

void UploadOctetStreamExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	const auto& [data, size, last] = request.getLargeData();


}

DECLARE_EXECUTOR(UploadOctetStreamExecutor)
