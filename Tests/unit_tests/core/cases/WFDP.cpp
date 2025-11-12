#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>

#include "utilities.h"

TEST(WFDP, Print)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parametersWithRoute
	(
		"wfdp",
		"data", "Hello, World!"
	).build
	(
		json::JsonBuilder(CP_UTF8).append("fileName", "print")
	);
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getBody(), "Hello, World!");
}

TEST(WFDP, For)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("wfdp").
		build
		(
			json::JsonBuilder(CP_UTF8).append("fileName", "for")
		);
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getBody(), "0123456789");
}

TEST(WFDP, Include)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder().getRequest().parameters("wfdp").
		build
		(
			json::JsonBuilder(CP_UTF8).append("fileName", "include")
		);
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_NE(web::HttpParser(response).getBody().find("<h1>Template</h1>"), std::string::npos);
}

TEST(WFDP, CustomFunction)
{
	streams::IOSocketStream stream = utility::createSocketStream();

	{
		std::string request = web::HttpBuilder().postRequest().parameters("wfdp").build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok);
	}

	{
		std::string request = web::HttpBuilder().getRequest().parametersWithRoute
		(
			"wfdp",
			"first", "15",
			"second", "30",
			"third", "45"
		).build
		(
			json::JsonBuilder(CP_UTF8).append("fileName", "custom_function")
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getBody(), "Data: 15 30 45");
	}

	{
		std::string request = web::HttpBuilder().deleteRequest().parameters("wfdp").build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok);
	}
}
