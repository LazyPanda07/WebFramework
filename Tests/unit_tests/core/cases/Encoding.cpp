#include <numeric>

#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>

#include "utilities.h"

using namespace std::string_literals;

TEST(Encoding, gzip)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("index.html").headers("Accept-Encoding", "gzip").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getHeaders().at("Content-Encoding"), "gzip");
	ASSERT_LT(web::HttpParser(response).getBody().size(), (std::ostringstream() << std::ifstream("assets/index.html", std::ios::binary).rdbuf()).str().size());
}

TEST(Encoding, brotli)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("index.html").headers("Accept-Encoding", "br").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getHeaders().at("Content-Encoding"), "br");
	ASSERT_LT(web::HttpParser(response).getBody().size(), (std::ostringstream() << std::ifstream("assets/index.html", std::ios::binary).rdbuf()).str().size());
}
