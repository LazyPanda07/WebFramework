#include <iostream>

#include "gtest/gtest.h"

#include "IOSocketStream.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"
#include "HTTPNetwork.h"

TEST(HelloExecutor, Get)
{
	try
	{
		streams::IOSocketStream stream(std::make_unique<web::HTTPNetwork>("127.0.0.1", "8080"));

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
	catch (const std::out_of_range&)
	{
		exit(-4);
	}
}
