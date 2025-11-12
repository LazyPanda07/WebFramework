#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>
#include <MultiLocalizationManager.h>

#include "utilities.h"

TEST(Localization, English)
{
	constexpr std::string_view language = "en";

	try
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HttpBuilder().getRequest().parameters("localization").build
		(
			json::JsonBuilder(CP_UTF8).append("language", language)
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getJson().get<std::string>("result"), localization::MultiLocalizationManager::getManager().getLocalizedString("LocalizationData", "key", language));
	}
	catch (const std::exception& e)
	{
		std::cout << "Localization error: " << e.what() << " on line: " << __LINE__ << std::endl;

		ASSERT_EQ(0, 1);
	}
}

TEST(Localization, Russian)
{
#ifdef __AARCH64__
	GTEST_SKIP();
#endif

	constexpr std::string_view language = "ru";

	try
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HttpBuilder().getRequest().parameters("localization").build
		(
			json::JsonBuilder(CP_UTF8).append("language", language)
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getJson().get<std::string>("result"), localization::MultiLocalizationManager::getManager().getLocalizedString("LocalizationData", "key", language));
	}
	catch (const std::exception& e)
	{
		std::cout << "Localization error: " << e.what() << " on line: " << __LINE__ << std::endl;

		ASSERT_EQ(0, 1);
	}
}
