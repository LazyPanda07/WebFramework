#include <fstream>
#include <format>

#include "gtest/gtest.h"

#include "settings.h"

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

	std::cout << "Url: " << std::format(R"(curl --header "Content-Type: application/octet-stream" --header "File-Name: octet_stream.bin" --data-binary @{} {}")", LARGE_FILE_NAME, (useHTTPS ? httpsUrl : httpUrl)) << std::endl;

	int errorCode = std::system(std::format(R"(curl --header "Content-Type: application/octet-stream" --header "File-Name: octet_stream.bin" --data-binary @{} {}")", LARGE_FILE_NAME, (useHTTPS ? httpsUrl : httpUrl)).data());

	ASSERT_EQ(errorCode, 0);
}
