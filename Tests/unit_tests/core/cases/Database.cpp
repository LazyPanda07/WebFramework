#include "gtest/gtest.h"

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>

#include "utilities.h"

TEST(Database, Create)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().postRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok) << response;
}

TEST(Database, Insert)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().putRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok) << response;
}

TEST(Database, Select)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	web::HttpParser parser(response);
	const json::JsonParser& json = parser.getJson();

	if (!json.contains<std::vector<json::JsonObject>>("data"))
	{
		std::cout << response << std::endl;
	}

	ASSERT_EQ(json.get<std::vector<json::JsonObject>>("data").size(), 11) << response;
}

TEST(Database, Update)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().patchRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getJson().get<std::vector<json::JsonObject>>("data").size(), 1) << response;
}

TEST(Database, Delete)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok) << response;
}
