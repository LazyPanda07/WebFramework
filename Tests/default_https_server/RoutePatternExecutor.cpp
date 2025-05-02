#include "RoutePatternExecutor.h"

void RoutePatternExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody
	(
		json::JSONBuilder(CP_UTF8).
		appendDouble("doubleValue", request.getRouteParameter<double>("doubleValue")).
		appendInt("integerValue", request.getRouteParameter<int64_t>("integerValue")).
		appendString("stringValue", request.getRouteParameter<std::string>("stringValue"))
	);
}

DECLARE_EXECUTOR(RoutePatternExecutor)
