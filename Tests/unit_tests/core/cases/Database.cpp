#include "gtest/gtest.h"

#include <format>
#include <array>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>

#include "utilities.h"

inline constexpr size_t databases =
#ifdef WITHOUT_POSTGRES_TESTS
1;
#else
2;
#endif

inline constexpr std::array<std::string_view, databases> databaseImplementations =
{
	"sqlite"
#ifndef WITHOUT_POSTGRES_TESTS
	, "postgres"
#endif
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

		ASSERT_EQ(web::HttpParser(response).getJson().get<std::vector<json::JsonObject>>("data").size(), 11) << response << std::endl << name;
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
