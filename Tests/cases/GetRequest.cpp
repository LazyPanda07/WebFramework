#include "gtest/gtest.h"

#include "IOSocketStream.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

TEST(HelloExecutor, Get)
{
	streams::IOSocketStream stream("127.0.0.1", "8080");
	std::string request = web::HTTPBuilder()
		.getRequest()
		.parameters("")
		.headers
		(
			"Host", "localhost"
		).build();
	std::string response;
	json::JSONParser parser;
	int64_t value;
	
	stream << request;

	stream >> response;

	parser.setJSONData(web::HTTPParser(response).getBody());

	ASSERT_EQ(parser.getString("message"), "Hello, World!");
	ASSERT_TRUE(parser.tryGetInt("randomNumber", value));
}
