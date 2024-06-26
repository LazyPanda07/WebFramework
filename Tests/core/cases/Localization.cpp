#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "utilities.h"

TEST(Localization, English)
{
	try
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HTTPBuilder().getRequest().parameters("localization").build
		(
			json::JSONBuilder(CP_UTF8).appendString("language", "en")
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HTTPParser(response).getJSON().getString("result"), "value");
	}
	catch (const std::exception& e)
	{
		std::cout << "Localization error: " << e.what() << " on line: " << __LINE__ << std::endl;

		ASSERT_EQ(0, 1);
	}
}

TEST(Localization, Russian)
{
	try
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HTTPBuilder().getRequest().parameters("localization").build
		(
			json::JSONBuilder(CP_UTF8).appendString("language", "ru")
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HTTPParser(response).getJSON().getString("result"), "значение");
	}
	catch (const std::exception& e)
	{
		std::cout << "Localization error: " << e.what() << " on line: " << __LINE__ << std::endl;

		ASSERT_EQ(0, 1);
	}
}
