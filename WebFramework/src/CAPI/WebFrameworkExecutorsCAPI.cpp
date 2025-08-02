#include "WebFrameworkExecutorsCAPI.h"

#include "Log.h"
#include "WebInterfaces/IHTTPRequest.h"

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
