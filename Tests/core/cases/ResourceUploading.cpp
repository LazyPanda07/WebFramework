#include <fstream>
#include <format>

#include "gtest/gtest.h"

#include "settings.h"
#include "utilities.h"

TEST(ResourceUploading, Chunk)
{

}

TEST(ResourceUploading, Multipart)
{
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_multipart";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_multipart";
	constexpr std::string_view firstFileName = "uploaded_api_test.py.py";
	constexpr std::string_view secondFileName = "uploaded_sqlite3.dll";
	constexpr std::string_view thirdFileName = "uploaded_web.json";

	int errorCode = std::system(std::format(R"(curl --insecure -X POST -F "{}=@api_test.py;type=text/plain" -F "{}=@sqlite3.dll" -F "{}=@web.json;type=application/json" {})", firstFileName, secondFileName, thirdFileName, (useHTTPS ? httpsUrl : httpUrl)).data());

	std::cout << utility::removeUploadedPrefix(firstFileName) << std::endl;

	ASSERT_EQ(errorCode, 0);
	ASSERT_TRUE(utility::compareFiles(firstFileName, utility::removeUploadedPrefix(firstFileName)));
	ASSERT_TRUE(utility::compareFiles(secondFileName, utility::removeUploadedPrefix(secondFileName)));
	ASSERT_TRUE(utility::compareFiles(thirdFileName, utility::removeUploadedPrefix(thirdFileName)));
}

TEST(ResourceUploading, OctetStream)
{
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_octet_stream";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_octet_stream";
	constexpr std::string_view uploadFileName = "octet_stream.bin";

	int errorCode = std::system(std::format(R"(curl --insecure --header "Content-Type: application/octet-stream" --header "File-Name: {}" --header "Expect: nothing" --data-binary @{} {})", uploadFileName, LARGE_FILE_NAME, (useHTTPS ? httpsUrl : httpUrl)).data());

	ASSERT_EQ(errorCode, 0);
	ASSERT_TRUE(utility::compareFiles(LARGE_FILE_NAME, uploadFileName));
}
