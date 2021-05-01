#include "BaseExecutor.h"

using namespace std;

namespace framework
{
	void BaseExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void BaseExecutor::doPost(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doGet(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doHead(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doPut(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doDelete(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doOptions(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doTrace(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}
}
