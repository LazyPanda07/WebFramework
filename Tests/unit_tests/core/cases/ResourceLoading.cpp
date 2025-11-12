#include <fstream>

#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <Strings.h>

#include "utilities.h"

TEST(ResourceLoading, StreamFile)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("download").build();
	std::string result = (std::ostringstream() << std::ifstream("assets/index.html", std::ios::binary).rdbuf()).str();
	std::string response;

	stream << request;

	stream >> response;

	std::unique_ptr<web::HttpParser> parser = std::make_unique<web::HttpParser>(response);

	ASSERT_EQ(parser->getBody(), result);

	ASSERT_EQ(parser->getHeaders().at("DownloadType"), "from-file");

	response.clear();

	stream << request;

	stream >> response;

	parser = std::make_unique<web::HttpParser>(response);

	ASSERT_EQ(parser->getBody(), result);

	ASSERT_EQ(parser->getHeaders().at("DownloadType"), "from-cache");
}

TEST(ResourceLoading, Index)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("index.html").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getBody(), (std::ostringstream() << std::ifstream("assets/index.html", std::ios::binary).rdbuf()).str());
}

TEST(ResourceLoading, PageMD)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string html;
	std::string md;

	{
		std::string request = web::HttpBuilder().getRequest().parameters("page.html").build();
		std::string response;

		stream << request;

		stream >> response;

		html = utility::strings::replaceAll(web::HttpParser(response).getBody(), "\r", "");
	}

	{
		std::string request = web::HttpBuilder().getRequest().parameters("page.md").build();
		std::string response;

		stream << request;

		stream >> response;

		md = web::HttpParser(response).getBody();
	}

	ASSERT_EQ(html, md) << html << std::endl << md;
}

TEST(ResourceLoading, DynamicResources)
{
	streams::IOSocketStream stream = utility::createSocketStream();

	{
		std::string request = web::HttpBuilder().postRequest().parameters("dynamic_resources").build
		(
			json::JsonBuilder(CP_UTF8).append("data", "Hello, World!")
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getBody(), "Hello, World!");
	}

	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string html;
		std::string md;

		{
			std::string request = web::HttpBuilder().getRequest().parameters("dynamic_resources").build();
			std::string response;

			stream << request;

			stream >> response;

			html = utility::strings::replaceAll(web::HttpParser(response).getBody(), "\r", "");
		}

		{
			std::string request = web::HttpBuilder().getRequest().parameters("page.md").build();
			std::string response;

			stream << request;

			stream >> response;

			md = web::HttpParser(response).getBody();
		}

		ASSERT_EQ(html, md) << html << std::endl << md;
	}
}
