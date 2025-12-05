#include "HelloExecutor.h"

void HelloExecutor::init(const framework::utility::ExecutorSettings& executorSettings)
{
	framework::JsonParser initParameters = executorSettings.getInitParameters();

	value = initParameters.get<int64_t>("number");
}

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	framework::JsonBuilder builder;

	builder["message"] = "Hello, World!";
	builder["number"] = value;

	response.setBody(builder);
}

void HelloExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doHead(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

DEFINE_EXECUTOR(HelloExecutor)
