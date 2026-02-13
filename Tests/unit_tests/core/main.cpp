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

json::JsonBuilder updateConfig(json::JsonParser&& configParser, const utility::parsers::ConsoleArgumentParser& consoleParser);

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser consoleParser(argc, argv);
	std::string serverConfig = consoleParser.getRequired<std::string>("server_config");
	json::JsonParser configParser = std::ifstream(serverConfig);

	useHTTPS = configParser.get<bool>("useHTTPS", true);

	std::ofstream(serverConfig) << updateConfig(std::move(configParser), consoleParser);

	unit_test_utils::ProcessWrapper server(unit_test_utils::ProcessWrapper(unit_test_utils::splitArguments(consoleParser.getRequired<std::string>("run_arguments"), serverConfig)));
	unit_test_utils::ProcessWrapper defaultHttpsServer = unit_test_utils::ProcessWrapper::runDefaultHttpsServer();

	testing::InitGoogleTest(&argc, argv);

	createLargeFile();

	auto start = std::chrono::high_resolution_clock::now();

#ifndef FLUTTER_API
	while (!std::filesystem::exists(START_CORE_SERVER_FILE))
	{
		if (std::filesystem::exists("error.txt") && std::filesystem::file_size("error.txt"))
		{
			std::cerr << std::ifstream("error.txt").rdbuf() << std::endl;

			exit(1);
		}

		std::cout << "Wait " << START_CORE_SERVER_FILE << " file..." << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count() > 1)
		{
			std::cout << "Still no " << START_CORE_SERVER_FILE << " file. Exit" << std::endl;

			break;
		}
	}
#else
	std::this_thread::sleep_for(std::chrono::seconds(5));
#endif

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

	exit(-1);
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

json::JsonBuilder updateConfig(json::JsonParser&& configParser, const utility::parsers::ConsoleArgumentParser& consoleParser)
{
	json::JsonBuilder result(CP_UTF8);

	{
		json::JsonObject object;

		configParser.getParsedData(object);

		result = json::JsonBuilder(object, CP_UTF8);

		if (!object.contains<std::vector<json::JsonObject>>("runtimes", true))
		{
			return result;
		}
	}

	json::JsonObject& webFramework = result["WebFramework"];

	for (const std::string& runtime : consoleParser.getValues<std::string>("runtimes"))
	{
		for (json::JsonObject& object : webFramework["runtimes"])
		{
			if (object["name"].get<std::string>() == runtime)
			{
				object["enabled"] = true;
			}
		}
	}

	return result;
}
