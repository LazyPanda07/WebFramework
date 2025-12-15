#include "HelloExecutor.h"

void HelloExecutor::init(const framework::utility::ExecutorSettings& executorSettings)
{
	framework::JsonParser initParameters = executorSettings.getInitParameters();

	value = initParameters.get<int64_t>("number");
}

void HelloExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::JsonBuilder builder;

	builder["message"] = "Hello, World!";
	builder["number"] = value;

	response.setBody(builder);
}

void HelloExecutor::doPost(framework::HttpRequest& request, framework::HttpResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doHead(framework::HttpRequest& request, framework::HttpResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doPut(framework::HttpRequest& request, framework::HttpResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doDelete(framework::HttpRequest& request, framework::HttpResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doPatch(framework::HttpRequest& request, framework::HttpResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doConnect(framework::HttpRequest& request, framework::HttpResponse& response)
{
	this->doGet(request, response);
}

DEFINE_EXECUTOR(HelloExecutor)
