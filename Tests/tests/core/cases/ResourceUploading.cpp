#include <fstream>
#include <format>

#include "gtest/gtest.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"

#include "settings.h"
#include "utilities.h"

TEST(ResourceUploading, Chunk)
{
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_chunked";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_chunked";
	constexpr std::string_view uploadFileName = "load_balancer_server.py";

	int errorCode = std::system(std::format(R"(curl --max-time 3600 --insecure --header "Transfer-Encoding: chunked" --header "File-Name: uploaded_{}" --data-binary @{} {})", uploadFileName, uploadFileName, (useHTTPS ? httpsUrl : httpUrl)).data());

	ASSERT_EQ(errorCode, 0);
	ASSERT_TRUE(utility::compareFiles(std::format("uploaded_{}", uploadFileName), uploadFileName));
}

TEST(ResourceUploading, Multipart)
{
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_multipart";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_multipart";
	constexpr std::string_view firstFileName = "uploaded_api_test.py";
	constexpr std::string_view secondFileName = "uploaded_web.json";

	int errorCode = std::system(std::format(R"(curl --max-time 3600 --insecure -X POST -F "{}=@api_test.py;type=text/plain" -F "{}=@web.json;type=application/json" {})", firstFileName, secondFileName, (useHTTPS ? httpsUrl : httpUrl)).data());

	ASSERT_EQ(errorCode, 0);
	ASSERT_TRUE(utility::compareFiles(firstFileName, utility::removeUploadedPrefix(firstFileName)));
	ASSERT_TRUE(utility::compareFiles(secondFileName, utility::removeUploadedPrefix(secondFileName)));
}

TEST(ResourceUploading, OctetStream)
{
#ifdef __AARCH64__
	GTEST_SKIP();
#endif
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_octet_stream";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_octet_stream";
	constexpr std::string_view uploadFileName = "octet_stream.bin";

	int errorCode = std::system(std::format(R"(curl --max-time 3600 --insecure -X POST {} -H "Content-Type: application/octet-stream" -H "File-Name: {}" --data-binary @{})", (useHTTPS ? httpsUrl : httpUrl), uploadFileName, LARGE_FILE_NAME).data());

	ASSERT_TRUE(utility::compareFiles(LARGE_FILE_NAME, uploadFileName));

	if (errorCode != 0)
	{
		std::cout << std::format("Exist finish_uploading.txt: {}", std::filesystem::exists("finish_uploading.txt")) << std::endl;
	}

	std::ostringstream os;
	std::ifstream file("log.txt");

	os << file.rdbuf();

	std::cout << os.str() << std::endl;

	ASSERT_EQ(errorCode, 0);
}
