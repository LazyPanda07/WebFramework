#include <fstream>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"

#include "utilities.h"

TEST(ResourceLoading, StreamFile)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("download").build();
	std::string result = (std::ostringstream() << std::ifstream("assets/index.html", std::ios::binary).rdbuf()).str();
	std::string response;

	stream << request;

	stream >> response;

	std::unique_ptr<web::HTTPParser> parser = std::make_unique<web::HTTPParser>(response);

	ASSERT_EQ(parser->getBody(), result);

	ASSERT_EQ(parser->getHeaders().at("DownloadType"), "from-file");

	response.clear();

	stream << request;

	stream >> response;

	parser = std::make_unique<web::HTTPParser>(response);

	ASSERT_EQ(parser->getBody(), result);

	ASSERT_EQ(parser->getHeaders().at("DownloadType"), "from-cache");
}

TEST(ResourceLoading, Index)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("index.html").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getBody(), (std::ostringstream() << std::ifstream("assets/index.html", std::ios::binary).rdbuf()).str());
}
