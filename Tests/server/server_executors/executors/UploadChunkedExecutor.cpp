#include "UploadChunkedExecutor.h"

void UploadChunkedExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::ofstream stream(request.getHeaders().at("File-Name"), std::ios::binary);

	for (const std::string& chunk : request.getChunks())
	{
		stream.write(chunk.data(), chunk.size());
	}

	response.setResponseCode(web::ResponseCodes::created);
	response.addBody("Finish uploading file");
}

DECLARE_EXECUTOR(UploadChunkedExecutor)
