#include "HTTPResponse.h"

using namespace std;

namespace framework
{
	HTTPResponse::HTTPResponse(interfaces::IHTTPResponse* implementation, const function<void(interfaces::IHTTPResponse*)>& deleter) :
		implementation(implementation),
		deleter(deleter)
	{

	}

	void HTTPResponse::setHTTPVersion(string_view version)
	{
		implementation->setHTTPVersion(version.data());
	}

	void HTTPResponse::setResponseCode(web::ResponseCodes code)
	{
		implementation->setResponseCode(code);
	}

	void HTTPResponse::addHeader(string_view name, string_view value)
	{
		implementation->addHeader(name.data(), value.data());
	}

	void HTTPResponse::setBody(string_view body)
	{
		implementation->setBody(body.data());
	}

	void HTTPResponse::setBody(const json::JSONBuilder& json)
	{
		implementation->setBody(json.build().data());
	}

	HTTPResponse& HTTPResponse::appendBody(string_view body)
	{
		implementation->appendBody(body.data());

		return *this;
	}

	void HTTPResponse::addCookie(string_view name, string_view value)
	{
		implementation->addCookie(name.data(), value.data());
	}

	void HTTPResponse::setDefault()
	{
		implementation->setDefault();
	}

	void HTTPResponse::setIsValid(bool isValid)
	{
		implementation->setIsValid(isValid);
	}

	HTTPResponse::operator bool() const
	{
		return implementation->getIsValid();
	}

	HTTPResponse::~HTTPResponse()
	{
		if (deleter && implementation)
		{
			deleter(implementation);

			implementation = nullptr;
		}
	}
}
