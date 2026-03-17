#include <array>
#include <thread>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>

#include <gtest/gtest.h>

#include <IOSocketStream.h>
#include <HttpNetwork.h>
#include <HttpBuilder.h>
#include <HttpParser.h>

#include <ProcessWrapper.h>

int64_t port = 64000;

TEST(TaskBroker, Internal)
{
	constexpr std::array<std::string_view, 4> endpoints =
	{
		"cxx",
		"cc",
		"python",
		"csharp"
	};
	auto getTxtFiles = []() -> size_t
		{
			size_t files = 0;

			for (const auto& it : std::filesystem::directory_iterator(std::filesystem::current_path()))
			{
				if (it.path().extension() == ".txt")
				{
					files++;
				}
			}

			return files;
		};

	streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HttpNetwork>("127.0.0.1", std::to_string(port));

	for (std::string_view endpoint : endpoints)
	{
		std::string response;
		json::JsonBuilder body(CP_UTF8);

		body["taskBroker"] = "internal";

		stream << web::HttpBuilder().getRequest().parameters(endpoint).build(body);

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::accepted);
	}

	auto start = std::chrono::steady_clock::now();

	while (true)
	{
		auto end = std::chrono::steady_clock::now();

		if (getTxtFiles() == 16 || (std::chrono::duration_cast<std::chrono::seconds>(end - start)).count() > 30)
		{
			break;
		}
	}

	ASSERT_EQ(getTxtFiles(), 16);
}

TEST(TaskBroker, RabbitMq)
{
	constexpr std::array<std::string_view, 4> endpoints =
	{
		"cxx",
		"cc",
		"python",
		"csharp"
	};
	auto getTxtFiles = []() -> size_t
		{
			size_t files = 0;

			for (const auto& it : std::filesystem::directory_iterator(std::filesystem::current_path()))
			{
				if (it.path().extension() == ".txt")
				{
					files++;
				}
			}

			return files;
		};

	streams::IOSocketStream stream = streams::IOSocketStream::createStream<web::HttpNetwork>("127.0.0.1", std::to_string(port));

	for (std::string_view endpoint : endpoints)
	{
		std::string response;
		json::JsonBuilder body(CP_UTF8);

		body["taskBroker"] = "rabbitmq";

		stream << web::HttpBuilder().getRequest().parameters(endpoint).build(body);

		stream >> response;

		ASSERT_EQ(web::HttpParser(response).getResponseCode(), web::ResponseCodes::accepted);
	}

	auto start = std::chrono::steady_clock::now();

	while (true)
	{
		auto end = std::chrono::steady_clock::now();

		if (getTxtFiles() == 32 || (std::chrono::duration_cast<std::chrono::seconds>(end - start)).count() > 30)
		{
			break;
		}
	}

	ASSERT_EQ(getTxtFiles(), 32);
}

void printLog();

int main(int argc, char** argv) try
{
	unit_test_utils::ProcessWrapper server(argv[1]);

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
