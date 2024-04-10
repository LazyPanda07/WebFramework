#include "BaseExecutor.h"

using namespace std;

namespace framework
{
	static void isImplemented
	(
		vector<string>& result,
		HTTPRequest& request, 
		const string& methodName, 
		void(BaseExecutor::*method)(HTTPRequest&, HTTPResponse&), 
		BaseExecutor& executor
	);

	void BaseExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void BaseExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
		vector<string> methods = { "OPTIONS" };
		string allowHeader;

		isImplemented(methods, request, "GET", &BaseExecutor::doGet, *this);
		isImplemented(methods, request, "POST", &BaseExecutor::doPost, *this);
		isImplemented(methods, request, "HEAD", &BaseExecutor::doHead, *this);
		isImplemented(methods, request, "PUT", &BaseExecutor::doPut, *this);
		isImplemented(methods, request, "DELETE", &BaseExecutor::doDelete, *this);
		isImplemented(methods, request, "PATCH", &BaseExecutor::doPatch, *this);
		isImplemented(methods, request, "TRACE", &BaseExecutor::doTrace, *this);

		for (size_t i = 0; i < methods.size(); i++) 
		{
			allowHeader += methods[i];

			if (i + 1 != methods.size()) 
			{
				allowHeader += ", ";
			}
		}

		response.addHeader("Allow", allowHeader);
	}

	void BaseExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void isImplemented
	(
		vector<string>& result,
		HTTPRequest& request, 
		const string& methodName, 
		void(BaseExecutor::*method)(HTTPRequest&, HTTPResponse&), 
		BaseExecutor& executor
	)
		HTTPResponse response;

		try
		{
			(executor.*method)(request, response);

			result.push_back(methodName);
		}
		catch (const exceptions::NotImplementedException&)
		{

		}
		catch (...)
		{
			result.push_back(methodName);
		}
	}
}
