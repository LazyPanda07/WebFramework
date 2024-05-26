#include <vector>
#include <future>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <thread>

#include "gtest/gtest.h"

#include "JSONParser.h"
#include "HTTPParser.h"
#include "HTTPBuilder.h"
#include "ConsoleArgumentParser.h"

#include "utilities/utilities.h"

static constexpr size_t connections = 8;
int64_t port;
bool useHTTPS;

TEST(LoadBalancer, ConnectionsHeuristic)
{
	std::vector<streams::IOSocketStream> streams;
	std::vector<int64_t> ids;
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
		ids.push_back(awaiter.get());
	}

	std::unordered_map<int64_t, size_t> unique;

	for (int64_t id : ids)
	{
		unique[id]++;
	}

	ASSERT_EQ(unique.size(), 2);

	size_t first = unique.begin()->second;
	size_t second = (++unique.begin())->second;

	ASSERT_EQ(first, second);
}

int main(int argc, char** argv)
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

	port = parser.get<int64_t>("--port");
	useHTTPS = parser.get<bool>("--useHTTPS");

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
