#include <array>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include <IOSocketStream.h>
#include <Http/HttpNetwork.h>
#include <Http/HttpsNetwork.h>
#include <HttpBuilder.h>
#include <HttpParser.h>

#include <ProcessWrapper.h>

TEST(MultiplePorts, HTTP)
{
	using namespace std::chrono_literals;

	streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::http::HttpNetwork>("127.0.0.1", "14500", 1h);
	std::string request = web::HttpBuilder().getRequest().parameters("multiple_ports").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getBody(), "14500");
}

TEST(MultiplePorts, HTTPS)
{
	using namespace std::chrono_literals;

	streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::http::HttpsNetwork>("127.0.0.1", "14700", 1h);
	std::string request = web::HttpBuilder().getRequest().parameters("multiple_ports").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getBody(), "14700");
}

void printLog();

int main(int argc, char** argv) try
{
	unit_test_utils::ProcessWrapper server(argv[1], argv[2]);

	testing::InitGoogleTest(&argc, argv);

	int result = RUN_ALL_TESTS();

	if (result)
	{
		printLog();
	}

	return result;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(1);
}

void printLog()
{
	for (const auto& it : std::filesystem::recursive_directory_iterator(std::filesystem::current_path() / "logs"))
	{
		if (it.is_regular_file())
		{
			std::ifstream log(it.path());

			std::cout << (std::ostringstream() << log.rdbuf()).str() << std::endl;
		}
	}
}
