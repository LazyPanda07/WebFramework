#include <fstream>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"

#include "utilities.h"

TEST(ResourceLoading, Index)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("index.html").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getBody(), (std::ostringstream() << std::ifstream("assets/index.html").rdbuf()).str());
}
