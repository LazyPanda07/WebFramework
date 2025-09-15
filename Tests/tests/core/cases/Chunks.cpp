#include <numeric>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"

#include "utilities.h"

using namespace std::string_literals;

TEST(Chunks, Data)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("chunks").build();
	std::string response;

	stream << request;

	stream >> response;

	web::HTTPParser parser(response);
	const std::vector<std::string>& chunks = parser.getChunks();

	std::string result = std::accumulate(chunks.begin(), chunks.end(), ""s);

	ASSERT_EQ(result, "Some information here");
}

TEST(Chunks, File)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().postRequest().parameters("chunks").build();
	std::string response;

	stream << request;

	stream >> response;

	web::HTTPParser parser(response);
	const std::vector<std::string>& chunks = parser.getChunks();

	std::string result = std::accumulate(chunks.begin(), chunks.end(), ""s);

	ASSERT_EQ(result, "Some information here");
	ASSERT_EQ(parser.getHeaders().at("Content-Disposition"), (R"(attachment; filename="file.txt")"));
}
