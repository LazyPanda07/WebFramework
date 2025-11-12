#include <future>
#include <random>
#include <ctime>

#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <JsonParser.h>
#include <JSONBuilder.h>

#include "utilities.h"

static constexpr size_t clientsNumber = 8;
static constexpr size_t requestsNumber = 25;

static std::string generateRandomString()
{
	std::mt19937_64 random(std::time(nullptr));
	std::string result;

	for (size_t i = 0; i < 128; i++)
	{
		result += static_cast<char>(random() % 26 + 'A');
	}

	return result;
}

TEST(Database, MultiUser)
{
	std::vector<streams::IOSocketStream> clients;
	std::vector<std::future<void>> awaiters;
	auto requests = [](streams::IOSocketStream& stream)
		{
			std::string request = web::HttpBuilder().postRequest().parameters("multi_user_database").build();
			std::string response;

			stream << request;

			stream >> response;

			ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok) << response;

			for (size_t i = 0; i < requestsNumber; i++)
			{
				request = web::HttpBuilder().putRequest().parameters("multi_user_database").build(json::JsonBuilder(CP_UTF8).append("data", generateRandomString()));

				stream << request;

				stream >> response;

				ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok) << response;
			}

			request = web::HttpBuilder().getRequest().parameters("multi_user_database").build();

			stream << request;

			stream >> response;

			web::HttpParser parser(response);

			ASSERT_EQ(parser.getResponseCode(), web::ResponseCodes::ok) << response;

			ASSERT_EQ(parser.getJson().get<std::vector<json::JsonObject>>("data").size(), requestsNumber) << response;
		};

	clients.reserve(clientsNumber);
	awaiters.reserve(clientsNumber);

	for (size_t i = 0; i < clientsNumber; i++)
	{
		awaiters.emplace_back
		(
			std::async
			(
				std::launch::async,
				requests,
				std::ref(clients.emplace_back(utility::createSocketStream()))
			)
		);
	}

	for (std::future<void>& awaiter : awaiters)
	{
		awaiter.wait();
	}
}
