#include <vector>
#include <future>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <thread>
#include <filesystem>
#include <array>

#include <gtest/gtest.h>

#include <JsonParser.h>
#include <HttpParser.h>
#include <HttpBuilder.h>
#include <ConsoleArgumentParser.h>
#include <ProcessWrapper.h>

#include "utilities/utilities.h"

constexpr size_t connections = 8;
int64_t port;
bool useHTTPS;
bool customHeuristic;

TEST(LoadBalancer, ConnectionsHeuristic)
{
	if (customHeuristic)
	{
		GTEST_SKIP();
	}

	std::vector<streams::IOSocketStream> streams;
	std::vector<int64_t> ids;
	std::vector<std::future<int64_t>> awaiters;
	auto call = [](streams::IOSocketStream& stream)
		{
			std::string request = web::HttpBuilder().getRequest().build();
			std::string response;

			stream << request;

			stream >> response;

			return web::HttpParser(response).getJson().get<int64_t>("id");
		};

	streams.reserve(connections);
	awaiters.reserve(connections);

	for (size_t i = 0; i < connections; i++)
	{
		awaiters.emplace_back
		(
			std::async(std::launch::async, call, std::ref(streams.emplace_back(utility::createSocketStream(port, useHTTPS))))
		);
	}

	for (std::future<int64_t>& awaiter : awaiters)
	{
		int64_t id = awaiter.get();

		ids.push_back(id);
	}

	std::unordered_map<int64_t, size_t> unique;

	for (int64_t id : ids)
	{
		unique[id]++;
	}

	ASSERT_EQ(unique.size(), 2);
}

TEST(LoadBalancer, CustomHeuristic)
{
	if (!customHeuristic)
	{
		GTEST_SKIP();
	}

	std::vector<streams::IOSocketStream> streams;
	std::set<int64_t> ids;
	std::vector<std::future<int64_t>> awaiters;
	std::mt19937_64 random(time(nullptr));
	auto call = [&random](streams::IOSocketStream& stream)
		{
			std::string request = web::HttpBuilder().getRequest().build();
			std::string response;

			std::this_thread::sleep_for(std::chrono::seconds(random() % 5));

			stream << request;

			stream >> response;

			return web::HttpParser(response).getJson().get<int64_t>("id");
		};

	streams.reserve(connections);
	awaiters.reserve(connections);

	for (size_t i = 0; i < connections; i++)
	{
		awaiters.emplace_back
		(
			std::async(std::launch::async, call, std::ref(streams.emplace_back(utility::createSocketStream(port, useHTTPS))))
		);
	}

	for (std::future<int64_t>& awaiter : awaiters)
	{
		ids.insert(awaiter.get());
	}

	ASSERT_EQ(ids.size(), 1);
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

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);
	std::vector<unit_test_utils::ProcessWrapper> loadBalancerServers;

	unit_test_utils::updateConfigRuntimes(parser.getRequired<std::string>("config"), parser);

	loadBalancerServers.reserve(2);

	port = parser.getRequired<int64_t>("port");
	useHTTPS = parser.get<bool>("useHTTPS");
	customHeuristic = parser.get<bool>("custom_heuristic");

	std::vector<std::string> loadBalancerRunArguments = unit_test_utils::splitArguments(parser.getRequired<std::string>("load_balancer_run_arguments"), "--config", parser.getRequired<std::string>("config"), "--port", port);

	if (parser.get<bool>("serversHTTPS"))
	{
		loadBalancerRunArguments.emplace_back("--serversHTTPS");

		loadBalancerServers.emplace_back(unit_test_utils::splitArguments(parser.getRequired<std::string>("load_balancer_run_arguments"), "--config", "load_balancer_config_https.json", "--port", 10002, "--type", "server"));
		loadBalancerServers.emplace_back(unit_test_utils::splitArguments(parser.getRequired<std::string>("load_balancer_run_arguments"), "--config", "load_balancer_config_https.json", "--port", 10003, "--type", "server"));
	}
	else
	{
		loadBalancerServers.emplace_back(unit_test_utils::splitArguments(parser.getRequired<std::string>("load_balancer_run_arguments"), "--config", "load_balancer_config.json", "--port", 10000, "--type", "server"));
		loadBalancerServers.emplace_back(unit_test_utils::splitArguments(parser.getRequired<std::string>("load_balancer_run_arguments"), "--config", "load_balancer_config.json", "--port", 10001, "--type", "server"));
	}

	if (customHeuristic)
	{
		loadBalancerRunArguments.emplace_back("--custom_heuristic");
	}

	unit_test_utils::ProcessWrapper loadBalancer(loadBalancerRunArguments);

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

	exit(-1);
}
