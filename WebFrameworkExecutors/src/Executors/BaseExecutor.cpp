#include "BaseExecutor.h"

#include "Strings.h"

using namespace std;

static void isImplemented
(
	vector<string>& result,
	framework::HTTPRequest& request, 
	framework::HTTPResponse& response,
	const string& methodName, 
	void(framework::BaseExecutor::*method)(framework::HTTPRequest&, framework::HTTPResponse&), 
	framework::BaseExecutor& executor
);

namespace framework
{
	void (BaseExecutor::* BaseExecutor::getMethod(string_view methodName))(HTTPRequest&, HTTPResponse&)
	{
		static const ::utility::strings::string_based_unordered_map<void(BaseExecutor::*)(HTTPRequest&, HTTPResponse&)> methods =
		{
			{ "GET", &BaseExecutor::doGet },
			{ "POST", &BaseExecutor::doPost },
			{ "HEAD", &BaseExecutor::doHead },
			{ "PUT", &BaseExecutor::doPut },
			{ "DELETE", &BaseExecutor::doDelete },
			{ "PATCH", &BaseExecutor::doPatch },
			{ "OPTIONS",&BaseExecutor::doOptions },
			{ "TRACE", &BaseExecutor::doTrace },
			{ "CONNECT", &BaseExecutor::doConnect }
		};

		return methods.find(methodName)->second;
	}

	void BaseExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void BaseExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
#ifdef NDEBUG
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
#endif

		vector<string> methods = { "OPTIONS" };
		string allowHeader;

		isImplemented(methods, request, response, "GET", &BaseExecutor::doGet, *this);
		isImplemented(methods, request, response, "POST", &BaseExecutor::doPost, *this);
		isImplemented(methods, request, response, "HEAD", &BaseExecutor::doHead, *this);
		isImplemented(methods, request, response, "PUT", &BaseExecutor::doPut, *this);
		isImplemented(methods, request, response, "DELETE", &BaseExecutor::doDelete, *this);
		isImplemented(methods, request, response, "PATCH", &BaseExecutor::doPatch, *this);
		isImplemented(methods, request, response, "TRACE", &BaseExecutor::doTrace, *this);
		isImplemented(methods, request, response, "CONNECT", &BaseExecutor::doConnect, *this);

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
#ifdef NDEBUG
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
#endif

		response.setBody(request.getRawRequest());
	}

	void BaseExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}
}

inline void isImplemented
(
	vector<string>& result,
	framework::HTTPRequest& request,
	framework::HTTPResponse& response,
	const string& methodName, 
	void(framework::BaseExecutor::*method)(framework::HTTPRequest&, framework::HTTPResponse&), 
	framework::BaseExecutor& executor
)
{
	response.setDefault();

	try
	{
		(executor.*method)(request, response);

		result.push_back(methodName);
	}
	catch (const framework::exceptions::NotImplementedException&)
	{

	}
	catch (...)
	{
		result.push_back(methodName);
	}
}
