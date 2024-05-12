#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "utilities.h"

TEST(WFDP, Print)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parametersWithRoute
	(
		"wfdp",
		"data", "Hello, World!"
	).build
	(
		json::JSONBuilder(CP_UTF8).appendString("fileName", "print.wfdp")
	);
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getBody(), "Hello, World!");
}

TEST(WFDP, For)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("wfdp").
		build
		(
			json::JSONBuilder(CP_UTF8).appendString("fileName", "for.wfdp")
		);
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getBody(), "0123456789");
}

TEST(WFDP, Include)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("wfdp").
		build
		(
			json::JSONBuilder(CP_UTF8).appendString("fileName", "include.wfdp")
		);
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_NE(web::HTTPParser(response).getBody().find("<h1>Template</h1>"), std::string::npos);
}

TEST(WFDP, CustomFunction)
{
	streams::IOSocketStream stream = utility::createSocketStream();

	{
		std::string request = web::HTTPBuilder().postRequest().parameters("wfdp").build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok);
	}

	{
		std::string request = web::HTTPBuilder().getRequest().parametersWithRoute
		(
			"wfdp",
			"first", "15",
			"second", "30",
			"third", "45"
		).build
		(
			json::JSONBuilder(CP_UTF8).appendString("fileName", "custom_function.wfdp")
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HTTPParser(response).getBody(), "Data: 15 30 45");
	}

	{
		std::string request = web::HTTPBuilder().deleteRequest().parameters("wfdp").build();
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok);
	}
}
