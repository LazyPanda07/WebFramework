#include "UploadMultipartExecutor.h"

#include <fstream>
#include <filesystem>

void UploadMultipartExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::filesystem::path currentPath = std::filesystem::current_path();

	for (const web::Multipart& multipart : request.getMultiparts())
	{
		std::ofstream file(currentPath / multipart.getName(), std::ios::binary);
		const std::string& data = multipart.getData();

		file.write(data.data(), data.size());
	}

	response.setResponseCode(web::ResponseCodes::created);
	response.setBody("Finish uploading files");
}

DECLARE_EXECUTOR(UploadMultipartExecutor)
