#include <iostream>

#include "WebFramework.h"

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
		request.sendAssetFile("index.wfdp", response, arguments);
	}

	void doPost(framework::HTTPRequest&& request, framework::HTTPResponse& response) override
	{
		response.addBody("<h1>Login: " + request.getKeyValueParameters().at("login") + "</h1><br><h1>Password: " + request.getKeyValueParameters().at("password") + "</h1>");
	}

	~DefaultRoute() = default;
};

int main(int argc, char** argv)
{
	try
	{
		framework::WebFramework webFramework("web_framework_settings.json");
		string s;

		webFramework.startServer();

		while (cin >> s)
		{

		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
