#include <iostream>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "utility.h"

TEST(HelloExecutor, Get)
{
	streams::IOSocketStream stream = utility::createSocketStream();

	for (size_t i = 0; i < 1000; i++)
	{
		std::string request = web::HTTPBuilder().getRequest().build();
		std::string response;
		json::JSONParser parser;
		int64_t value;

		stream << request;

		stream >> response;

		parser.setJSONData(web::HTTPParser(response).getBody());

		ASSERT_EQ(parser.getString("message"), "Hello, World!");
		ASSERT_TRUE(parser.tryGetInt("randomNumber", value));
	}
}
