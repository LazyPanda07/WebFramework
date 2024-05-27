#include "gtest/gtest.h"

#include "HTTPParser.h"
#include "HTTPBuilder.h"
#include "ConsoleArgumentParser.h"

#include "utilities.h"

bool useHTTPS;
int64_t port;

TEST(Proxy, DefaultRoute)
{
	streams::IOSocketStream stream = utility::createSocketStream(port, useHTTPS);
	std::string request = web::HTTPBuilder().getRequest().build();
	std::string response;
	int64_t id;

	stream << request;

	stream >> response;

	ASSERT_TRUE(web::HTTPParser(response).getJSON().tryGetInt("id", id));
}

TEST(Proxy, DefaultRequestRoute)
{
	streams::IOSocketStream stream = utility::createSocketStream(port, useHTTPS);
	std::string request = web::HTTPBuilder().getRequest().parameters("default_request").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getJSON().getString("data"), "data");
}

int main(int argc, char** argv)
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

	useHTTPS = parser.get<bool>("useHTTPS");
	port = parser.get<int64_t>("port");

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
