#include "UploadMultipartExecutor.h"

#include <fstream>

void UploadMultipartExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	for (const framework::Multipart& multipart : request.getMultiparts())
	{
		std::ofstream file(multipart.getName(), std::ios::binary);
		const std::string& data = multipart.getData();

		file.write(data.data(), data.size());
	}

	response.setResponseCode(framework::ResponseCodes::created);
	response.setBody("Finish uploading files");
}

DECLARE_EXECUTOR(UploadMultipartExecutor)
