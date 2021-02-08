#include <iostream>

#include "WebFramework.h"

#pragma comment (lib, "WebFramework.lib")

using namespace std;

class Test : public framework::BaseStatelessExecutor
{
private:
	vector<int64_t> numbers;

public:
	void init(const framework::utility::XMLSettingsParser::ExecutorSettings& settings) override
	{
		for (size_t i = 0; i < 5; i++)
		{
			numbers.push_back(rand());
		}
	}

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override
	{
		response.addBody
		(
			json::JSONBuilder(1251).push_back<string>(make_pair("user"s, "admin")).push_back<string>(make_pair("password"s, "qweqwe123123"s)).push_back<vector<int64_t>>(make_pair("balance"s, numbers)).build()
		);
	}
};

CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(Test)

class Parameters : public framework::BaseStatelessExecutor
{
public:
	void init(const framework::utility::XMLSettingsParser::ExecutorSettings& settings) override
	{

	}

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override
	{
		response.addBody
		(
			"<h1>Hello</h1>"
		);
	}
};

CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(Parameters);

int main(int argc, char** argv)
{
	try
	{
		framework::WebFramework webFramework("web_framework_settings.ini");
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
