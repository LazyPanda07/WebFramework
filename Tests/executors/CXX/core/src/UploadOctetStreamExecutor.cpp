#include "UploadOctetStreamExecutor.h"

void UploadOctetStreamExecutor::init(const framework::utility::ExecutorSettings& settings)
{
	currentSize = 0;
}

void UploadOctetStreamExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	const auto& [data, last] = request.getLargeData();
	int64_t contentLength = stoll(request.getHeaders().at("Content-Length"));

	if (!stream.is_open())
	{
		stream.open(request.getHeaders().at("File-Name"), std::ios::binary);
	}

	stream.write(data.data(), data.size());

	currentSize += data.size();

	if (currentSize == contentLength)
	{
		std::ofstream("finish_uploading.txt");
	}

	if (last)
	{
		stream.close();

		response.setResponseCode(framework::ResponseCodes::created);
		response.setBody("Finish uploading file");
	}
}

DEFINE_EXECUTOR(UploadOctetStreamExecutor)
