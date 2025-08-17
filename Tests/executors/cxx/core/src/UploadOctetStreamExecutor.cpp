#include "UploadOctetStreamExecutor.h"

#include "Log.h"

void UploadOctetStreamExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	const auto& [data, size, last] = request.getLargeData();

	if (!stream.is_open())
	{
		stream.open(request.getHeaders().at("File-Name"), std::ios::binary);
	}

	if (Log::isValid())
	{
		Log::info("Write {} bytes", "LogUploadOctetStreamExecutor", data.size());
	}

	stream.write(data.data(), data.size());

	if (last)
	{
		if (Log::isValid())
		{
			Log::info("Last upload iteration", "LogUploadOctetStreamExecutor");
		}

		stream.close();

		response.setResponseCode(framework::ResponseCodes::created);
		response.setBody("Finish uploading file");
	}
}

DECLARE_EXECUTOR(UploadOctetStreamExecutor)
