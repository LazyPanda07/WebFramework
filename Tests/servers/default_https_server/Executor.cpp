#include "Executor.h"

void Executor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	response.setBody(framework::JsonBuilder().append("data", "data"));
}

DEFINE_EXECUTOR(Executor)
