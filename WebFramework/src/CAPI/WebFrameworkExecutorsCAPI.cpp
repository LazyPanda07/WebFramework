#include "WebFrameworkExecutorsCAPI.h"

#include "Log.h"
#include "WebInterfaces/IHTTPRequest.h"
#include "JSONBuilder.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {}", "C_API", e.what()); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error("Something went wrong");

void setHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setBody(body);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setHTTPResponseJSONBody(HTTPResponseObject response, JSONBuilder builder, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setBody(static_cast<json::JSONBuilder*>(builder)->build().data());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setHTTPVersion(HTTPResponseObject response, const char* version, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setHTTPVersion(version);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setHTTPResponseCode(HTTPResponseObject response, int64_t responseCode, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setResponseCode(responseCode);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void addHTTPResponseHeader(HTTPResponseObject response, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->addHeader(name, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->appendBody(body);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void addHTTPResponseCookie(HTTPResponseObject response, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->addCookie(name, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setHTTPResponseDefault(HTTPResponseObject response, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setDefault();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setHTTPResponseIsValid(HTTPResponseObject response, bool isValid, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setIsValid(isValid);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}
