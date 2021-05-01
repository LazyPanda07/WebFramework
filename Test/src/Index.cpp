#include "Index.h"

using namespace std;

void Index::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
{

}

void Index::doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response)
{
	if (!request.isDynamicFunctionRegistered("br"))
	{
		request.registerDynamicFunction("br", [](const vector<string>& values) -> string { return values[0] + "<br>"; });
	}

	request.sendAssetFile("/index.wfdp", response);
}
