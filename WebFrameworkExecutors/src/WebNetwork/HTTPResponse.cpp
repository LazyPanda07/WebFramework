#include "HTTPResponse.h"

using namespace std;

namespace framework
{
	interfaces::IHTTPResponse* HTTPResponse::getImplementation() const
	{
		return implementation;
	}

	HTTPResponse::HTTPResponse(interfaces::IHTTPResponse* implementation, const function<void(interfaces::IHTTPResponse*)>& deleter) :
		implementation(implementation),
		deleter(deleter)
	{

	}

	HTTPResponse::HTTPResponse(HTTPResponse&& other) noexcept
	{
		(*this) = move(other);
	}

	HTTPResponse& HTTPResponse::operator =(HTTPResponse&& other) noexcept
	{
		implementation = other.implementation;
		deleter = other.deleter;

		other.implementation = nullptr;
		other.deleter = nullptr;

		return *this;
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
		implementation->addHeader("Content-Type", "application/json");

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
