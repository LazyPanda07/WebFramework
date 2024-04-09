#include "HelloExecutor.h"

#include "JSONBuilder.h"

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	json::JSONBuilder builder(CP_UTF8);

	builder["message"] = "Hello, World!";
	builder["randomNumber"] = rand();

	response.addBody(builder);
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

void HelloExecutor::doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

void HelloExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	this->doGet(request, response);
}

DECLARE_EXECUTOR(HelloExecutor)
