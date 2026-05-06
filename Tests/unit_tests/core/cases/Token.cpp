#include <gtest/gtest.h>

#include <format>

#include <HttpBuilder.h>
#include <HttpParser.h>

#include "utilities.h"

static constexpr std::string_view userName = "LazyPanda07";

static inline std::string token;

TEST(Token, GetToken)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	json::JsonBuilder data;

	data["userName"] = userName;

	std::string request = web::HttpBuilder().postRequest().parameters("get_token").build(data);
	std::string response;

	stream << request;
	
	stream >> response;

	web::HttpParser parser(response);

	ASSERT_EQ(parser.getResponseCode(), 200);

	token = parser.getJson().get<std::string>("token");
}

TEST(Token, WithoutToken)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("token").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getResponseCode(), 403);
}

TEST(Token, WithToken)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder()
		.getRequest()
		.parameters("token")
		.headers
		(
			"Authorization", std::format("Bearer {}", token)
		)
		.build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getBody(), userName);
}
