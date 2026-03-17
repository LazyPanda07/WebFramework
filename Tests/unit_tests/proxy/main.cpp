#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include <HttpParser.h>
#include <HttpBuilder.h>
#include <ConsoleArgumentParser.h>
#include <ProcessWrapper.h>
#include <UnitTestUtils.h>

#include "utilities.h"

int64_t port;
bool useHTTPS;

TEST(Proxy, DefaultRoute)
{
	streams::IOSocketStream stream = utility::createSocketStream(port, useHTTPS);
	std::string request = web::HttpBuilder().getRequest().build();
	std::string response;
	int64_t id;

	stream << request;

	stream >> response;

	ASSERT_TRUE(web::HttpParser(response).getJson().tryGet<int64_t>("id", id));
}

TEST(Proxy, DefaultRequestRoute)
{
	streams::IOSocketStream stream = utility::createSocketStream(port, useHTTPS);
	std::string request = web::HttpBuilder().getRequest().parameters("default_request").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HttpParser(response).getJson().get<std::string>("data"), "data");
}

void printLog();

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);

	port = parser.getRequired<int64_t>("port");
	useHTTPS = parser.get<bool>("useHTTPS");

	unit_test_utils::updateConfigRuntimes("load_balancer_config.json", parser);
	unit_test_utils::updateConfigRuntimes("proxy_config.json", parser);

	std::vector<std::string> loadBalancerRunArguments = unit_test_utils::splitArguments(parser.getRequired<std::string>("load_balancer_run_arguments"), "--config", "load_balancer_config.json", "--type", "server", "--port", 10000);
	std::vector<std::string> proxyRunArguments = unit_test_utils::splitArguments(parser.getRequired<std::string>("proxy_run_arguments"), "--config", "proxy_config.json", "--port", port);

	if (useHTTPS)
	{
		proxyRunArguments.emplace_back("--useHTTPS");
	}

	unit_test_utils::ProcessWrapper defaultHttpsServer = unit_test_utils::ProcessWrapper::runDefaultHttpsServer();
	unit_test_utils::ProcessWrapper loadBalancer(loadBalancerRunArguments);
	unit_test_utils::ProcessWrapper proxy(proxyRunArguments);

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
