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

}

TEST(ResourceUploading, OctetStream)
{
	constexpr std::string_view httpUrl = "http://127.0.0.1:8080/upload_octet_stream";
	constexpr std::string_view httpsUrl = "https://127.0.0.1:8080/upload_octet_stream";
	constexpr std::string_view uploadFileName = "octet_stream.bin";

	int errorCode = std::system(std::format(R"(curl --insecure --header "Content-Type: application/octet-stream" --header "File-Name: {}" --data-binary @{} {})", uploadFileName, LARGE_FILE_NAME, (useHTTPS ? httpsUrl : httpUrl)).data());

	ASSERT_EQ(errorCode, 0);
	ASSERT_TRUE(utility::compareFiles(LARGE_FILE_NAME, uploadFileName));
}
