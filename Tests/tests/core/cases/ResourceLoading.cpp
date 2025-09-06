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

TEST(ResourceLoading, PageMD)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string html;
	std::string md;

	{
		std::string request = web::HTTPBuilder().getRequest().parameters("page.html").build();
		std::string response;

		stream << request;

		stream >> response;

		html = web::HTTPParser(response).getBody();
	}

	{
		std::string request = web::HTTPBuilder().getRequest().parameters("page.md").build();
		std::string response;

		stream << request;

		stream >> response;

		md = web::HTTPParser(response).getBody();
	}

	ASSERT_EQ(html, md) << html << std::endl << md;
}
