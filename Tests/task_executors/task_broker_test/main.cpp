#include <array>
#include <thread>
#include <chrono>
#include <filesystem>
#include <format>

#include <gtest/gtest.h>

#include <IOSocketStream.h>
#include <HttpNetwork.h>
#include <HttpBuilder.h>
#include <HttpParser.h>
#include <reproc++/run.hpp>

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

		stream << web::HttpBuilder().getRequest().parameters(endpoint).build();

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

class Wrapper
{
private:
	reproc::process process;

public:
	Wrapper(const std::vector<std::string>& arguments);

	~Wrapper();
};

int main(int argc, char** argv) try
{
	Wrapper server({ argv[1] });

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(1);
}

Wrapper::Wrapper(const std::vector<std::string>& arguments)
{
	constexpr size_t outputBufferSize = 256;

	{
		std::error_code error = process.start(reproc::arguments(arguments));

		if (error)
		{
			throw std::runtime_error(std::format("Error while creating new process: {}", error.message()));
		}
	}

	std::vector<uint8_t> buffer(outputBufferSize, 0);

	auto [_, error] = process.read(reproc::stream::out, buffer.data(), buffer.size());

	if (error)
	{
		throw std::runtime_error(std::format("Error while running new process: {}", error.message()));
	}
}

Wrapper::~Wrapper()
{
	process.kill();
}
