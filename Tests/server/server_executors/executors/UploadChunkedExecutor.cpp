#include "UploadChunkedExecutor.h"

void UploadChunkedExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	for (const web::Multipart& multipart : request.getMultiparts())
	{
		std::ofstream file;
		const std::string& data = multipart.getData();

		if (multipart.getContentType() == "application/json" || multipart.getContentType() == "text/plain")
		{
			file.open(multipart.getName());
		}
		else
		{
			file.open(multipart.getName(), std::ios::binary);
		}

		file.write(data.data(), data.size());
	}
}

DECLARE_EXECUTOR(UploadChunkedExecutor)
