#include <numeric>
#include <chrono>
#include <filesystem>

#include <gtest/gtest.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <IOSocketStream.h>
#include <HttpsNetwork.h>

using namespace std::chrono_literals;

TEST(UserAgentFilter, PerExecutor)
{
    streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HttpsNetwork>("127.0.0.1", "20000", 1h);

    {
        std::string request = web::HttpBuilder().getRequest().parameters("user_agent").build();
	    std::string response;

	    stream << request;

	    stream >> response;

        ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::forbidden);
    }

    {
        std::string request = web::HttpBuilder().getRequest().headers("User-Agent", "WrongUserAgent").parameters("user_agent").build();
        std::string response;

        stream << request;

        stream >> response;

        ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::forbidden);
    }

    {
        std::string request = web::HttpBuilder().getRequest().headers("User-Agent", "CustomUserAgent").parameters("user_agent").build();
	    std::string response;

	    stream << request;

	    stream >> response;

        ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok);
    }
}

TEST(UserAgentFilter, ConfigLevel)
{
    streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HttpsNetwork>("127.0.0.1", "20001", 1h);

    {
        std::string request = web::HttpBuilder().getRequest().parameters("default_request").build();
        std::string response;

        stream << request;

        stream >> response;

        ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::forbidden);
    }

    {
        std::string request = web::HttpBuilder().getRequest().headers("User-Agent", "WrongUserAgent").parameters("default_request").build();
        std::string response;

        stream << request;

        stream >> response;

        ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::forbidden);
    }

    {
        std::string request = web::HttpBuilder().getRequest().headers("User-Agent", "CustomUserAgent").parameters("default_request").build();
        std::string response;

        stream << request;

        stream >> response;

        ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::ok);
    }
}
