#include <iostream>

#include "WebFramework.h"
#include "Executors/BaseStatefulExecutor.h"

#pragma comment (lib, "WebFramework.lib")

using namespace std;

class DefaultRoute : public framework::BaseStatelessExecutor
{
private:
	string helloMessage;

public:
	DefaultRoute() = default;

	void init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings) override
	{
		helloMessage = settings.initParameters.getString("message");
	}

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override
	{
		unordered_map<string_view, string> arguments =
		{
			{ "title"sv, "Main page" },
			{ "message"sv, helloMessage }
		};

		request.sendAssetFile("index.wfdp", response, arguments);
	}

	~DefaultRoute() = default;
};

EXECUTOR_CREATION_FUNCTION(DefaultRoute)

class AnotherRoute : public framework::BaseStatefulExecutor
{
public:
	AnotherRoute()
	{
		cout << "Create AnotherRoute for user" << endl;
	}

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override
	{
		localization::TextLocalization& localization = localization::TextLocalization::get(); 
		unordered_map<string_view, string> arguments =
		{
			{ "title"sv, localization["title"] },
			{ "message"sv, localization["ipMessage"] + request.getClientIpV4()}
		};

		request.sendAssetFile("index.wfdp", response, arguments);
	}

	void destroy() override
	{
		cout << "Destroy AnotherRoute for user" << endl;
	}

	~AnotherRoute() = default;
};

EXECUTOR_CREATION_FUNCTION(AnotherRoute)

int main(int argc, char** argv)
{
	try
	{
		framework::WebFramework webFramework("web_framework_settings.json");
		string s;
		localization::TextLocalization::get().changeLanguage("ru");

		webFramework.startServer();

		while (cin >> s)
		{

		}
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
