#include "RoutePatternExecutor.h"

void RoutePatternExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	response.setBody
	(
		framework::JsonBuilder().
		append("doubleValue", request.getRouteParameter<double>("doubleValue")).
		append("integerValue", request.getRouteParameter<int64_t>("integerValue")).
		append("stringValue", request.getRouteParameter<std::string>("stringValue"))
	);
}

DEFINE_EXECUTOR(RoutePatternExecutor)
