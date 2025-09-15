#include "BaseExecutor.h"

#include "Strings.h"

using namespace std;

static void isImplemented
(
	vector<string>& result,
	framework::HTTPRequestExecutors& request, 
	framework::HTTPResponseExecutors& response,
	const string& methodName, 
	void(framework::BaseExecutor::*method)(framework::HTTPRequestExecutors&, framework::HTTPResponseExecutors&), 
	framework::BaseExecutor& executor
);

namespace framework
{
	void (BaseExecutor::* BaseExecutor::getMethod(string_view methodName))(HTTPRequestExecutors&, HTTPResponseExecutors&)
	{
		static const ::utility::strings::string_based_unordered_map<void(BaseExecutor::*)(HTTPRequestExecutors&, HTTPResponseExecutors&)> methods =
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

	void BaseExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
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

	void BaseExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
#ifdef NDEBUG
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
#endif

		response.setBody(request.getRawRequest());
	}

	void BaseExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}
}

void isImplemented
(
	vector<string>& result,
	framework::HTTPRequestExecutors& request,
	framework::HTTPResponseExecutors& response,
	const string& methodName, 
	void(framework::BaseExecutor::*method)(framework::HTTPRequestExecutors&, framework::HTTPResponseExecutors&), 
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
