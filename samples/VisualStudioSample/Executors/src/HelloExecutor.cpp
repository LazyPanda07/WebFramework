#include "HelloExecutor.h"

#include "JSONBuilder.h"

void HelloExecutor::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
{

}

void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody(json::JSONBuilder(CP_UTF8).appendString("message", "Hello, World!"));
}

void HelloExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void HelloExecutor::doHead(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void HelloExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void HelloExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void HelloExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void HelloExecutor::doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	BaseStatelessExecutor::doOptions(request, response);
}

void HelloExecutor::doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	BaseStatelessExecutor::doTrace(request, response);
}

void HelloExecutor::doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

DECLARE_EXECUTOR(HelloExecutor)
