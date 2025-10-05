#include <vector>
#include <future>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <thread>
#include <filesystem>
#include <array>

#include "gtest/gtest.h"

#include "JSONParser.h"
#include "HTTPParser.h"
#include "HTTPBuilder.h"
#include "ConsoleArgumentParser.h"

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
			std::string request = web::HTTPBuilder().getRequest().build();
			std::string response;

			stream << request;

			stream >> response;

			return web::HTTPParser(response).getJSON().getInt("id");
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
			std::string request = web::HTTPBuilder().getRequest().build();
			std::string response;

			std::this_thread::sleep_for(std::chrono::seconds(random() % 5));

			stream << request;

			stream >> response;

			return web::HTTPParser(response).getJSON().getInt("id");
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

int main(int argc, char** argv)
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

	port = parser.get<int64_t>("--port");
	useHTTPS = parser.get<bool>("--useHTTPS");
	customHeuristic = parser.get<bool>("--custom_heuristic");

	testing::InitGoogleTest(&argc, argv);

	constexpr std::array<std::string_view, 4> loadBalancers =
	{
		START_LOAD_BALANCER_9090_SERVER_FILE,
		START_LOAD_BALANCER_9091_SERVER_FILE,
		START_LOAD_BALANCER_9092_SERVER_FILE,
		START_LOAD_BALANCER_9093_SERVER_FILE
	};

	auto start = std::chrono::high_resolution_clock::now();

	while (!std::ranges::all_of(loadBalancers, [](std::string_view file) { return std::filesystem::exists(file); }))
	{
		std::cout << "Wait for all load balancers..." << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		auto end = std::chrono::high_resolution_clock::now();

		if (std::chrono::duration_cast<std::chrono::minutes>(end - start).count() > 5)
		{
			printLog();

			return -1;
		}
	}

	int result = RUN_ALL_TESTS();

	if (result)
	{
		printLog();
	}

	return result;
}
