#include "Executor.h"

#include <Strings.h>

static void isImplemented
(
	std::vector<std::string>& result,
	framework::interfaces::IHTTPRequest& request, 
	framework::interfaces::IHTTPResponse& response,
	const std::string& methodName, 
	void(framework::Executor::*method)(framework::interfaces::IHTTPRequest&, framework::interfaces::IHTTPResponse&), 
	framework::Executor& executor
);

namespace framework
{
	void (Executor::* Executor::getMethod(std::string_view methodName))(interfaces::IHTTPRequest&, interfaces::IHTTPResponse&)
	{
		static const ::utility::strings::string_based_unordered_map<void(Executor::*)(interfaces::IHTTPRequest&, interfaces::IHTTPResponse&)> methods =
		{
			{ "GET", &Executor::doGet },
			{ "POST", &Executor::doPost },
			{ "HEAD", &Executor::doHead },
			{ "PUT", &Executor::doPut },
			{ "DELETE", &Executor::doDelete },
			{ "PATCH", &Executor::doPatch },
			{ "OPTIONS",&Executor::doOptions },
			{ "TRACE", &Executor::doTrace },
			{ "CONNECT", &Executor::doConnect }
		};

		return methods.find(methodName)->second;
	}

	void Executor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void Executor::doPost(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Executor::doGet(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Executor::doHead(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Executor::doPut(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Executor::doDelete(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Executor::doPatch(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Executor::doOptions(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
#ifdef NDEBUG
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
#endif

		std::vector<std::string> methods = { "OPTIONS" };
		std::string allowHeader;

		isImplemented(methods, request, response, "GET", &Executor::doGet, *this);
		isImplemented(methods, request, response, "POST", &Executor::doPost, *this);
		isImplemented(methods, request, response, "HEAD", &Executor::doHead, *this);
		isImplemented(methods, request, response, "PUT", &Executor::doPut, *this);
		isImplemented(methods, request, response, "DELETE", &Executor::doDelete, *this);
		isImplemented(methods, request, response, "PATCH", &Executor::doPatch, *this);
		isImplemented(methods, request, response, "TRACE", &Executor::doTrace, *this);
		isImplemented(methods, request, response, "CONNECT", &Executor::doConnect, *this);

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

	void Executor::doTrace(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
	{
#ifdef NDEBUG
		throw exceptions::NotImplementedException(typeid(*this).name(), __func__);
#endif

		response.setBody(request.getRawRequest());
	}

	void Executor::doConnect(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response)
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
	void(framework::Executor::*method)(framework::interfaces::IHTTPRequest&, framework::interfaces::IHTTPResponse&), 
	framework::Executor& executor
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
