#include "RoutePatternExecutor.h"

void RoutePatternExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody
	(
		framework::JSONBuilder().
		append("doubleValue", request.getRouteParameter<double>("doubleValue")).
		append("integerValue", request.getRouteParameter<int64_t>("integerValue")).
		append("stringValue", request.getRouteParameter<std::string>("stringValue"))
	);
}

DECLARE_EXECUTOR(RoutePatternExecutor)
