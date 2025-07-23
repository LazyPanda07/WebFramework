#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "HTTPSNetwork.h"

#include "utilities.h"

static inline constexpr size_t requestsNumber = 100;

#define BASIC_TEST(requestType) \
TEST(HelloExecutor, requestType) \
{ \
	streams::IOSocketStream stream = utility::createSocketStream(); \
 \
	for (size_t i = 0; i < requestsNumber; i++) \
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
		ASSERT_TRUE(parser.tryGetInt("number", value)); \
		ASSERT_EQ(value, 890); \
	} \
}

std::string constructRequest(std::string_view requestType)
{
	web::HTTPBuilder result;

	if(requestType == "GET")
	{
		result.getRequest();
	}
	else if(requestType == "POST")
	{
		result.postRequest();
	}
	else if(requestType == "HEAD")
	{
		result.headRequest();
	}
	else if(requestType == "PUT")
	{
		result.putRequest();
	}
	else if(requestType == "DELETE")
	{
		result.deleteRequest();
	}
	else if(requestType == "PATCH")
	{
		result.patchRequest();
	}
	else if(requestType == "CONNECT")
	{
		result.connectRequest();
	}
	else if(requestType == "OPTIONS")
	{
		result.optionsRequest();
	}
	else if(requestType == "TRACE")
	{
		result.traceRequest();
	}

	return result.build();
}

TEST(HelloExecutor, OPTIONS)
{
	streams::IOSocketStream stream = utility::createSocketStream();

	for (size_t i = 0; i < requestsNumber; i++)
	{
		std::string request = constructRequest("OPTIONS");
		std::string response;

		stream << request;

		stream >> response;

		web::HTTPParser parser(response);

#ifdef NDEBUG
		ASSERT_EQ(parser.getResponseCode(), web::ResponseCodes::badRequest);
#else
		ASSERT_EQ(parser.getHeaders().at("Allow"), "OPTIONS, GET, POST, HEAD, PUT, DELETE, PATCH, TRACE, CONNECT");
#endif
	}
}

TEST(HelloExecutor, TRACE)
{
	streams::IOSocketStream stream = utility::createSocketStream();

	for (size_t i = 0; i < requestsNumber; i++)
	{
		std::string request = constructRequest("TRACE");
		std::string response;

		stream << request;

		stream >> response;

		web::HTTPParser parser(response);

#ifdef NDEBUG
		ASSERT_EQ(parser.getResponseCode(), web::ResponseCodes::badRequest);
#else
		std::istringstream is(request);
		std::string temp;
		const std::string& rawData = parser.getRawData();

		while (std::getline(is, temp))
		{
			ASSERT_NE(rawData.find(temp), std::string::npos);
		}
#endif
	}
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

BASIC_TEST(PATCH)

BASIC_TEST(CONNECT)

TEST(RoutePattern, PassingValues)
{
	streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "20000");
	std::string request;
	std::string response;

	request = web::HTTPBuilder().getRequest().parameters(std::format("pattern/{}/{}/{}", "qwe", 200, 25.5)).build();

	stream << request;

	stream >> response;

	json::JSONParser parser = web::HTTPParser(response).getJSON();

	ASSERT_EQ(parser.getString("stringValue"), "qwe");
	ASSERT_EQ(parser.getInt("integerValue"), 200);
	ASSERT_FLOAT_EQ(parser.getDouble("doubleValue"), 25.5);
}
