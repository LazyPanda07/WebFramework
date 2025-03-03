#include <fstream>
#include <format>

#include "gtest/gtest.h"

TEST(ResourceUploading, Chunk)
{

}

TEST(ResourceUploading, Multipart)
{

}

TEST(ResourceUploading, OctetStream)
{
	int errorCode = std::system(std::format(R"(curl --header "Content-Type: application/octet-stream" --data-binary @{} --header "File-Name: octet_stream.bin")", LARGE_FILE_NAME).data());

	ASSERT_EQ(errorCode, 0);
}
