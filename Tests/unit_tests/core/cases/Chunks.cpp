#include <numeric>

#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>

#include "utilities.h"

using namespace std::string_literals;

TEST(Chunks, Data)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("chunks").build();
	std::string response;

	stream << request;

	stream >> response;

	web::HttpParser parser(response);
	const std::vector<std::string>& chunks = parser.getChunks();

	std::string result = std::accumulate(chunks.begin(), chunks.end(), ""s);

	ASSERT_EQ(result, "Some information here");
}

TEST(Chunks, File)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().postRequest().parameters("chunks").build();
	std::string response;

	stream << request;

	stream >> response;

	web::HttpParser parser(response);
	const std::vector<std::string>& chunks = parser.getChunks();

	std::string result = std::accumulate(chunks.begin(), chunks.end(), ""s);

	ASSERT_EQ(result, "Some information here");
	ASSERT_EQ(parser.getHeaders().at("Content-Disposition"), (R"(attachment; filename="file.txt")"));
}
