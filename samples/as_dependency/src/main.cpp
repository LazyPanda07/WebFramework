#include <filesystem>

#include <import.hpp>
#include <Executors/StatelessExecutor.hpp>
#include <gtest/gtest.h>
#include <curl/curl.h>

static inline bool started = false;

int main(int argc, char** argv) try
{
	constexpr std::string_view config = R"({
  "WebServer": {
    "ip": "127.0.0.1",
    "port": 8080,
    "timeout": 0
  },
  "WebFramework": {
    "settingsPaths": [
      "web.json"
    ],
    "loadSources": [
      "current"
    ],
    "assetsPath": "assets",
    "templatesPath": "templates",
    "webServerType": "multiThreaded",
  }
})";

	framework::utility::initializeWebFramework();

	if (!std::filesystem::exists("web.json"))
	{
		std::ofstream("web.json") << R"({
  "Executor": {
    "route": "",
    "loadType": "initialization",
    "api": "cxx"
  }
})";
	}

	framework::WebFramework server(config, std::filesystem::current_path().string());

	server.start(false, []() { started = true; });

	while (!started);

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	return 1;
}

TEST(As_Dependency, GET)
{
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080");

	CURLcode result = curl_easy_perform(curl);

	ASSERT_EQ(result, CURLE_OK);

	curl_easy_cleanup(curl);
}

class Executor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override
	{
		framework::JsonBuilder result;

		result["message"] = "Hello, World!";

		response.setBody(result);
	}
};

DEFINE_EXECUTOR(Executor);
