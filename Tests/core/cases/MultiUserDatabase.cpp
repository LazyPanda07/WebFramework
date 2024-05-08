#include <future>
#include <random>
#include <ctime>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"
#include "JSONBuilder.h"

#include "utilities.h"

static constexpr size_t clientsNumber = 8;
static constexpr size_t requestsNumber = 5000;

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
        std::string request = web::HTTPBuilder().postRequest().parameters("database").build();
	    std::string response;

	    stream << request;

	    stream >> response;

	    ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok) << response;

        for (size_t i = 0; i < requestsNumber; i++)
        {
            request = web::HTTPBuilder().putRequest().parameters("database").build(json::JSONBuilder(CP_UTF8).appendString("data", generateRandomString()));

            stream << request;

            stream >> response;

            ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok) << response;
        }

        request = web::HTTPBuilder().getRequest().parameters("database").build();

        stream << request;

        stream >> response;

        web::HTTPParser parser(response);

        ASSERT_EQ(parser.getResponseCode(), web::responseCodes::ok) << response;

        ASSERT_EQ(parser.getJSON().getArray("data").size(), requestsNumber) << response;
    };

    clients.reserve(clientsNumber);
    awaiters.reserve(clientsNumber);

    for (size_t i = 0; i < clientsNumber; i++)
    {
        streams::IOSocketStream& stream = clients.emplace_back(utility::createSocketStream());

        awaiters.emplace_back(std::async(std::launch::async, requests, std::ref(stream)));
    }

    for (std::future<void>& awaiter : awaiters)
    {
        awaiter.wait();
    }
}
