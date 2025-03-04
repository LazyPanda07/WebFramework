#include "UploadMultipartExecutor.h"

void UploadMultipartExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::filesystem::path currentPath = std::filesystem::current_path();

	for (const web::Multipart& multipart : request.getMultiparts())
	{
		std::ofstream file;
		const std::string& data = multipart.getData();

		if (multipart.getContentType() == "application/json" || multipart.getContentType() == "text/plain")
		{
			file.open(currentPath / multipart.getName());
		}
		else
		{
			file.open(currentPath / multipart.getName(), std::ios::binary);
		}

		file.write(data.data(), data.size());
	}

	response.setResponseCode(web::ResponseCodes::created);
	response.addBody("Finish uploading files");
}

DECLARE_EXECUTOR(UploadMultipartExecutor)
