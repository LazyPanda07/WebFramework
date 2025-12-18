#include "UploadChunkedExecutor.h"

#include <fstream>

void UploadChunkedExecutor::doPost(framework::HttpRequest& request, framework::HttpResponse& response)
{
	std::ofstream stream(request.getHeaders().at("File-Name"), std::ios::binary);

	for (const std::string& chunk : request.getChunks())
	{
		stream.write(chunk.data(), chunk.size());
	}

	response.setResponseCode(framework::ResponseCodes::created);
	response.setBody("Finish uploading file");
}

DEFINE_EXECUTOR(UploadChunkedExecutor)
