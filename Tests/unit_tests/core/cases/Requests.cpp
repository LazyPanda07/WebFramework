#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>

#include <HttpsNetwork.h>

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
		json::JsonParser parser; \
		int64_t value; \
 \
		stream << request; \
 \
		stream >> response; \
 \
		parser.setJSONData(web::HttpParser(response).getBody()); \
 \
		ASSERT_EQ(parser.get<std::string>("message"), "Hello, World!"); \
		ASSERT_TRUE(parser.tryGet<int64_t>("number", value)); \
		ASSERT_EQ(value, 890); \
	} \
}

std::string constructRequest(std::string_view requestType)
{
	web::HttpBuilder result;

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
	else if (requestType == "PATCH")
	{
		result.patchRequest();
	}
	else if (requestType == "CONNECT")
	{
		result.connectRequest();
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

TEST(HelloExecutor, OPTIONS)
{
	streams::IOSocketStream stream = utility::createSocketStream();

	for (size_t i = 0; i < requestsNumber; i++)
	{
		std::string request = constructRequest("OPTIONS");
		std::string response;

		stream << request;

		stream >> response;

		web::HttpParser parser(response);

		ASSERT_TRUE
		(
			parser.getResponseCode() == web::ResponseCodes::badRequest ||
			parser.getHeaders().at("Allow") == "OPTIONS, GET, POST, HEAD, PUT, DELETE, PATCH, TRACE, CONNECT"
		) << response;
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

		web::HttpParser parser(response);

		bool result = parser.getResponseCode() == web::ResponseCodes::badRequest ||
			[&]() -> bool
			{
				std::istringstream is(request);
				std::string temp;
				const std::string& rawData = parser.getRawData();

				while (std::getline(is, temp))
				{
					if (rawData.find(temp) == std::string::npos)
					{
						return false;
					}
				}

				return true;
			}();

		ASSERT_TRUE(result) << response;
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
	using namespace std::chrono_literals;

	auto start = std::chrono::high_resolution_clock::now();

	while (!std::filesystem::exists(START_DEFAULT_HTTPS_SERVER_FILE))
	{
		std::cout << "Wait " << START_DEFAULT_HTTPS_SERVER_FILE << " file..." << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count() > 1)
		{
			break;
		}
	}

	streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HttpsNetwork>("127.0.0.1", "20000", 1h);
	std::string request;
	std::string response;

	request = web::HttpBuilder().getRequest().parameters(std::format("pattern/{}/{}/{}", "qwe", 200, 25.5)).build();

	stream << request;

	stream >> response;

	json::JsonParser parser = web::HttpParser(response).getJson();

	ASSERT_EQ(parser.get<std::string>("stringValue"), "qwe");
	ASSERT_EQ(parser.get<int>("integerValue"), 200);
	ASSERT_FLOAT_EQ(parser.get<double>("doubleValue"), 25.5);
}
