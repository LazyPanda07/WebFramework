#include <iostream>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "utilities.h"

#define BASIC_TEST(requestType) \
TEST(HelloExecutor, requestType) \
{ \
	streams::IOSocketStream stream = utility::createSocketStream(); \
 \
	for (size_t i = 0; i < 10000; i++) \
	{ \
		std::string request = constructRequest(#requestType); \
		std::string response; \
		json::JSONParser parser; \
		int64_t value; \
 \
		stream << request; \
 \
		stream >> response; \
 \
		parser.setJSONData(web::HTTPParser(response).getBody()); \
 \
		ASSERT_EQ(parser.getString("message"), "Hello, World!"); \
		ASSERT_TRUE(parser.tryGetInt("randomNumber", value)); \
	} \
}

std::string constructRequest(std::string_view requestType)
{
	web::HTTPBuilder result;

	if (requestType == "GET")
	{
		result.getRequest();
	}
	else if (requestType == "POST")
	{
		result.postRequest();
	}
	else if (requestType == "HEAD")
	{
		result.headRequest();
	}
	else if (requestType == "PUT")
	{
		result.putRequest();
	}
	else if (requestType == "DELETE")
	{
		result.deleteRequest();
	}
	else if (requestType == "OPTIONS")
	{
		result.optionsRequest();
	}
	else if (requestType == "TRACE")
	{
		result.traceRequest();
	}

	return result.build();
}

BASIC_TEST(GET)

BASIC_TEST(POST)

BASIC_TEST(HEAD)

BASIC_TEST(PUT)

#define TEMP DELETE

#undef DELETE

BASIC_TEST(DELETE)

#define DELETE TEMP

#undef TEMP

BASIC_TEST(OPTIONS)

BASIC_TEST(TRACE)
