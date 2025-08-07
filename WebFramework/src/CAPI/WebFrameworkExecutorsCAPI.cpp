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

const char* getHTTPRawParameters(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getRawParameters();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getHTTPMethod(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getMethod();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void* getHTTPVersion(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return new std::string(std::format("HTTP/{}", static_cast<framework::interfaces::IHTTPRequest*>(request)->getHTTPVersion()));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getHTTPBody(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getBody();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void setHTTPAttribute(HTTPRequestObject request, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->setAttribute(name, value);
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

void* getHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception)
{
	try
	{
		framework::interfaces::IHTTPRequest* implementation = static_cast<framework::interfaces::IHTTPRequest*>(request);
		const char* temp = implementation->getAttribute(name);
		std::string* result = new std::string(temp);

		implementation->deleteAttribute(temp);

		return result;
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void deleteHTTPSession(HTTPRequestObject request, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->deleteSession();
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

void removeHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->removeAttribute(name);
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

JSONParser getHTTPRequestJSON(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return createJSONParserFromString(static_cast<framework::interfaces::IHTTPRequest*>(request)->getJSON(), exception);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getHTTPRawRequest(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getRawRequest();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void* getClientIpV4(HTTPRequestObject request, Exception* exception)
{
	try
	{
		framework::interfaces::IHTTPRequest* implementation = static_cast<framework::interfaces::IHTTPRequest*>(request);
		const char* temp = implementation->getClientIpV4();
		std::string* result = new std::string(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void* getServerIpV4(HTTPRequestObject request, Exception* exception)
{
	try
	{
		framework::interfaces::IHTTPRequest* implementation = static_cast<framework::interfaces::IHTTPRequest*>(request);
		const char* temp = implementation->getServerIpV4();
		std::string* result = new std::string(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

uint16_t getClientPort(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getClientPort();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

uint16_t getServerPort(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getServerPort();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}
