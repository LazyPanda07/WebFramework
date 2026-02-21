#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>

#include <HttpsNetwork.h>

#include "utilities.h"
#include "settings.h"

TEST(Redis, CommonOperations)
{
	if (!runRedisTests)
	{
		GTEST_SKIP();
	}

	streams::IOSocketStream stream = utility::createSocketStream();
	std::string response;
	
	stream << web::HttpBuilder().postRequest().parameters("redis");
	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::created);

	response.clear();

	stream << web::HttpBuilder().putRequest().parameters("redis");
	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::created);

	response.clear();

	stream << web::HttpBuilder().getRequest().parameters("redis");
	stream >> response;

	web::HttpParser parser(response);

	ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok) << response;

	json::JsonParser body(parser.getBody());

	ASSERT_EQ(body.get<std::string>("string"), "qwe");
	ASSERT_EQ(body.get<int>("int"), 5);
	ASSERT_EQ(body.get<double>("double"), 2.3);
	ASSERT_EQ(body.get<bool>("bool"), true);
}
