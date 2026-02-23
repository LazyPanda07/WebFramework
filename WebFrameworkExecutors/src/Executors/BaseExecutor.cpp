#include "BaseExecutor.h"

#include <Strings.h>

static void isImplemented
(
	std::vector<std::string>& result,
	framework::interfaces::IHTTPRequest& request, 
	framework::interfaces::IHTTPResponse& response,
	const std::string& methodName, 
	void(framework::BaseExecutor::*method)(framework::interfaces::IHTTPRequest&, framework::interfaces::IHTTPResponse&), 
	framework::BaseExecutor& executor
);

namespace framework
{
	void (BaseExecutor::* BaseExecutor::getMethod(std::string_view methodName))(interfaces::IHTTPRequest&, interfaces::IHTTPResponse&)
	{
		static const ::utility::strings::string_based_unordered_map<void(BaseExecutor::*)(interfaces::IHTTPRequest&, interfaces::IHTTPResponse&)> methods =
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

	void BaseExecutor::doPost(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doGet(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doHead(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doPut(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doDelete(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doPatch(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void BaseExecutor::doOptions(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
#ifdef NDEBUG
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
#endif

		std::vector<std::string> methods = { "OPTIONS" };
		std::string allowHeader;

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

		response.addHeader("Allow", allowHeader.data());
	}

	void BaseExecutor::doTrace(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
#ifdef NDEBUG
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
#endif

		response.setBody(request.getRawRequest());
	}

	void BaseExecutor::doConnect(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}
}

void isImplemented
(
	std::vector<std::string>& result,
	framework::interfaces::IHTTPRequest& request,
	framework::interfaces::IHTTPResponse& response,
	const std::string& methodName, 
	void(framework::BaseExecutor::*method)(framework::interfaces::IHTTPRequest&, framework::interfaces::IHTTPResponse&), 
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
