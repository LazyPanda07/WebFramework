#include "UploadOctetStreamExecutor.h"

#include "Log.h"

void UploadOctetStreamExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	const auto& [data, size, last] = request.getLargeData();

	if (!stream.is_open())
	{
		stream.open(request.getHeaders().at("File-Name"), std::ios::binary);
	}

	if (data.size() < 2000)
	{
		Log::info("Size: , data: {}", "LogUploadOctetStreamExecutor", data.size(), data);
	}
	
	stream.write(data.data(), data.size());

	Log::info("Upload data size: {}, body size: {}, last: {}", "LogUploadOctetStreamExecutor", data.size(), size, last);

	if (last)
	{
		stream.close();

		response.setResponseCode(web::ResponseCodes::created);
		response.setBody("Finish uploading file");
	}
}

DECLARE_EXECUTOR(UploadOctetStreamExecutor)
