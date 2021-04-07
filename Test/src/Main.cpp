#include <iostream>

#include "WebFramework.h"
#include "Utility/Memory.h"

#pragma comment (lib, "WebFramework.lib")

using namespace std;

class Test : public framework::BaseStatelessExecutor
{
public:
	void init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings) override
	{

	}

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override
	{
		auto variables = framework::utility::make_smartPointer<unordered_map<string_view, string>>();

		variables->insert(make_pair("test", "test"));

		request.sendAssetFile("/index.wfdp", response, variables);
	}
};

CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(Test)

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
