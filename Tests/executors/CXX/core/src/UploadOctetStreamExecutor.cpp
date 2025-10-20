#include "UploadOctetStreamExecutor.h"

void UploadOctetStreamExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	const auto& [data, last] = request.getLargeData();

	if (!stream.is_open())
	{
		stream.open(request.getHeaders().at("File-Name"), std::ios::binary);
	}

	stream.write(data.data(), data.size());

	if (last)
	{
		stream.close();

		response.setResponseCode(framework::ResponseCodes::created);
		response.setBody("Finish uploading file");
	}
}

DEFINE_EXECUTOR(UploadOctetStreamExecutor)
