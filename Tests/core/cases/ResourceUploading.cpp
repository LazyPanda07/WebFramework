#include <fstream>
#include <format>

#include "gtest/gtest.h"
#include "HTTPBuilder.h"

#include "settings.h"
#include "utilities.h"

TEST(ResourceUploading, Chunk)
{
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_chunked";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_chunked";
	constexpr std::string_view uploadFileName = "load_balancer_server.py";

	int errorCode = std::system(std::format(R"(curl --insecure --header "Transfer-Encoding: chunked" --header "File-Name: uploaded_{}" --data-binary @{} {})", uploadFileName, uploadFileName, (useHTTPS ? httpsUrl : httpUrl)).data());

	ASSERT_EQ(errorCode, 0);
	ASSERT_TRUE(utility::compareFiles(std::format("uploaded_{}", uploadFileName), uploadFileName));
}

TEST(ResourceUploading, Multipart)
{
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_multipart";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_multipart";
	constexpr std::string_view firstFileName = "uploaded_api_test.py";
	constexpr std::string_view secondFileName = "uploaded_web.json";

	int errorCode = std::system(std::format(R"(curl --insecure -X POST -F "{}=@api_test.py;type=text/plain" -F "{}=@web.json;type=application/json" {})", firstFileName, secondFileName, (useHTTPS ? httpsUrl : httpUrl)).data());

	ASSERT_EQ(errorCode, 0);
	ASSERT_TRUE(utility::compareFiles(firstFileName, utility::removeUploadedPrefix(firstFileName)));
	ASSERT_TRUE(utility::compareFiles(secondFileName, utility::removeUploadedPrefix(secondFileName)));
}

TEST(ResourceUploading, OctetStream)
{
	constexpr std::string_view uploadFileName = "octet_stream.bin";
	uintmax_t fileSize = std::filesystem::file_size(LARGE_FILE_NAME);
	constexpr size_t chunkSize = 10ULL * 1024 * 1024;

	streams::IOSocketStream stream = utility::createSocketStream();
	std::string headers = web::HTTPBuilder().postRequest().parameters("upload_octet_stream").headers
	(
		"File-Name", LARGE_FILE_NAME,
		"Content-Type", "application/octet-stream",
		"Content-Length", fileSize
	).build();

	stream << headers;

	std::ifstream in(LARGE_FILE_NAME, std::ios::binary);
	std::string data(chunkSize, '\0');

	for (uintmax_t i = 0; i < fileSize; i++)
	{
		size_t readSize = std::min<size_t>(chunkSize, fileSize - i);
		
		data.reserve(readSize);

		in.read(data.data(), readSize);

		stream << data;
	}

	ASSERT_TRUE(utility::compareFiles(LARGE_FILE_NAME, uploadFileName));
}
