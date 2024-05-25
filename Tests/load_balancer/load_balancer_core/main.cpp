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

#include "utilities/utilities.h"

static constexpr size_t connections = 8;
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
			std::async(std::launch::async, call, std::ref(streams.emplace_back(utility::createSocketStream(useHTTPS))))
		);
	}

	for (std::future<int64_t>& awaiter : awaiters)
	{
		ids.push_back(awaiter.get());
	}

	ASSERT_EQ
	(
		std::ranges::count(ids, 1),
		std::ranges::count(ids, 2)
	);
}

int main(int argc, char** argv)
{
	useHTTPS = json::JSONParser(std::ifstream(argv[1])).getObject("WebFramework").getObject("HTTPS").getBool("useHTTPS");

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
