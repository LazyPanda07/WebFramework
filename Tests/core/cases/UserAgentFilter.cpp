#include <numeric>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"

#include "utilities.h"

TEST(UserAgentFilter, PerExecutor)
{
    streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "20000");

    {
        std::string request = web::HTTPBuilder().getRequest().parameters("user_agent").build();
	    std::string response;

	    stream << request;

	    stream >> response;

        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::forbidden);
    }

    {
        std::string request = web::HTTPBuilder().getRequest().headers("User-Agent", "CustomUserAgent").parameters("user_agent").build();
	    std::string response;

	    stream << request;

	    stream >> response;

        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::ok);
    }
}
