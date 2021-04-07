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
		if (!request.isDynamicFunctionRegistered("br"))
		{
			request.registerDynamicFunction("br", [](const vector<string>& values) ->string { return values[0] + "<br>"; });
		}

		request.sendAssetFile("/index.wfdp", response);
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
