#include <fstream>
#include <filesystem>
#include <chrono>
#include <random>

#include <JsonParser.h>
#include <JsonBuilder.h>
#include <ConsoleArgumentParser.h>

#include <gtest/gtest.h>

#include <ProcessWrapper.h>
#include <UnitTestUtils.h>

#include "settings.h"

bool useHTTPS;
constexpr size_t largeFileSize = 200 * 1024 * 1024;
constexpr size_t fileChunkSize = largeFileSize / 256;
constexpr size_t randomNumbers = fileChunkSize / sizeof(size_t);

void printLog();

void createLargeFile();

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser consoleParser(argc, argv);
	std::string serverConfig = consoleParser.getRequired<std::string>("server_config");
	json::JsonParser configParser = std::ifstream(serverConfig);

	useHTTPS = configParser.get<bool>("useHTTPS", true);

	unit_test_utils::updateConfigRuntimes(serverConfig, consoleParser);

	unit_test_utils::ProcessWrapper defaultHttpsServer = unit_test_utils::ProcessWrapper::runDefaultHttpsServer();

	if (consoleParser.get<bool>("manual"))
	{
		testing::InitGoogleTest(&argc, argv);

		createLargeFile();

		int result = RUN_ALL_TESTS();

		if (result)
		{
			printLog();
		}

		return 0;
	}

	unit_test_utils::ProcessWrapper server(unit_test_utils::ProcessWrapper(unit_test_utils::splitArguments(consoleParser.getRequired<std::string>("run_arguments"), serverConfig)));
	
	testing::InitGoogleTest(&argc, argv);

	createLargeFile();

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

	printLog();

	return -1;
}

void printLog()
{
	for (const auto& it : std::filesystem::recursive_directory_iterator(std::filesystem::current_path() / "logs"))
	{
		if (it.is_regular_file())
		{
			std::ifstream log(it.path());

			std::cerr << (std::ostringstream() << log.rdbuf()).str() << std::endl;
		}
	}
}

void createLargeFile()
{
	if (std::filesystem::exists(LARGE_FILE_NAME))
	{
		return;
	}

	std::ofstream out(LARGE_FILE_NAME, std::ios::binary);
	size_t currentSize = 0;
	std::string data(fileChunkSize, '\0');
	std::mt19937_64 random;

	while (currentSize != largeFileSize)
	{
		for (size_t i = 0; i < randomNumbers; i++)
		{
			size_t number = random();
			const char* ptr = reinterpret_cast<const char*>(&number);

			std::copy(ptr, ptr + sizeof(size_t), data.data() + i * sizeof(size_t));
		}

		out.write(data.data(), data.size());

		currentSize += fileChunkSize;
	}
}
