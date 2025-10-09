#include <numeric>
#include <chrono>
#include <filesystem>

#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "IOSocketStream.h"
#include "HTTPSNetwork.h"

using namespace std::chrono_literals;

// TODO: libpython3.13.so.1.0
//TEST(UserAgentFilter, PerExecutor)
//{
//    auto start = std::chrono::high_resolution_clock::now();
//
//    while (!std::filesystem::exists(START_DEFAULT_HTTPS_SERVER_FILE))
//    {
//        std::cout << "Wait " << START_DEFAULT_HTTPS_SERVER_FILE << " file..." << std::endl;
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        if (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count() > 1)
//        {
//            break;
//        }
//    }
//
//    streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "20000", 1h);
//
//    {
//        std::string request = web::HTTPBuilder().getRequest().parameters("user_agent").build();
//	    std::string response;
//
//	    stream << request;
//
//	    stream >> response;
//
//        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::forbidden);
//    }
//
//    {
//        std::string request = web::HTTPBuilder().getRequest().headers("User-Agent", "WrongUserAgent").parameters("user_agent").build();
//        std::string response;
//
//        stream << request;
//
//        stream >> response;
//
//        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::forbidden);
//    }
//
//    {
//        std::string request = web::HTTPBuilder().getRequest().headers("User-Agent", "CustomUserAgent").parameters("user_agent").build();
//	    std::string response;
//
//	    stream << request;
//
//	    stream >> response;
//
//        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::ok);
//    }
//}
//
//TEST(UserAgentFilter, ConfigLevel)
//{
//    auto start = std::chrono::high_resolution_clock::now();
//
//    while (!std::filesystem::exists(START_DEFAULT_HTTPS_SERVER_FILE))
//    {
//        std::cout << "Wait " << START_DEFAULT_HTTPS_SERVER_FILE << " file..." << std::endl;
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        if (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count() > 1)
//        {
//            break;
//        }
//    }
//
//    streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "20001", 1h);
//
//    {
//        std::string request = web::HTTPBuilder().getRequest().parameters("default_request").build();
//        std::string response;
//
//        stream << request;
//
//        stream >> response;
//
//        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::forbidden);
//    }
//
//    {
//        std::string request = web::HTTPBuilder().getRequest().headers("User-Agent", "WrongUserAgent").parameters("default_request").build();
//        std::string response;
//
//        stream << request;
//
//        stream >> response;
//
//        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::forbidden);
//    }
//
//    {
//        std::string request = web::HTTPBuilder().getRequest().headers("User-Agent", "CustomUserAgent").parameters("default_request").build();
//        std::string response;
//
//        stream << request;
//
//        stream >> response;
//
//        ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::ResponseCodes::ok);
//    }
//}
