#include "gtest/gtest.h"

#include <format>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>

#include "utilities.h"

inline constexpr std::array<std::string_view, 1> databaseImplementations =
{
	"sqlite"
};

TEST(Database, Insert)
{
	for (std::string_view name : databaseImplementations)
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HttpBuilder().putRequest().parameters(std::format("database/{}", name)).build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::noContent) << response;
	}
}

TEST(Database, Select)
{
	for (std::string_view name : databaseImplementations)
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HttpBuilder().getRequest().parameters(std::format("database/{}", name)).build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getJson().get<std::vector<json::JsonObject>>("data").size(), 11) << response;
	}
}

TEST(Database, Update)
{
	for (std::string_view name : databaseImplementations)
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HttpBuilder().patchRequest().parameters(std::format("database/{}", name)).build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getJson().get<std::vector<json::JsonObject>>("data").size(), 1) << response;
	}
}

TEST(Database, Delete)
{
	for (std::string_view name : databaseImplementations)
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HttpBuilder().getRequest().parameters(std::format("database/{}", name)).build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok) << response;
	}
}
