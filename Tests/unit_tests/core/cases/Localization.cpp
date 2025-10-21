#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "utilities.h"

#include "MultiLocalizationManager.h"

TEST(Localization, English)
{
	constexpr std::string_view language = "en";

	try
	{
		streams::IOSocketStream stream = utility::createSocketStream();
		std::string request = web::HTTPBuilder().getRequest().parameters("localization").build
		(
			json::JSONBuilder(CP_UTF8).appendString("language", language)
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HTTPParser(response).getJSON().getString("result"), localization::MultiLocalizationManager::getManager().getLocalizedString("LocalizationData", "key", language));
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
		std::string request = web::HTTPBuilder().getRequest().parameters("localization").build
		(
			json::JSONBuilder(CP_UTF8).appendString("language", language)
		);
		std::string response;

		stream << request;

		stream >> response;

		ASSERT_EQ(web::HTTPParser(response).getJSON().getString("result"), localization::MultiLocalizationManager::getManager().getLocalizedString("LocalizationData", "key", language));
	}
	catch (const std::exception& e)
	{
		std::cout << "Localization error: " << e.what() << " on line: " << __LINE__ << std::endl;

		ASSERT_EQ(0, 1);
	}
}
