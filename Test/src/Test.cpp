#include "Test.h"

using namespace std;

void Test::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
{

}

void Test::doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response)
{
	response.addBody
	(
		"<h1>Test</h1>"
	);
}
