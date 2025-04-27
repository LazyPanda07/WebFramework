#include "HTTPResponse.h"

using namespace std;

namespace framework
{
	string HTTPResponse::getFullDate()
	{
		return format("{:%a, %d %b %Y %H:%M:%OS} GMT", chrono::system_clock::now());
	}

	HTTPResponse::HTTPResponse()
	{
		this->setDefault();
	}

	HTTPResponse& HTTPResponse::operator = (const web::HTTPBuilder& builder)
	{
		this->builder = builder;

		return *this;
	}

	void HTTPResponse::setHTTPVersion(const char* version)
	{
		builder.HTTPVersion(version);
	}

	void HTTPResponse::setResponseCode(web::ResponseCodes code)
	{
		builder.responseCode(code);
	}

	void HTTPResponse::addHeader(const char* name, const char* value)
	{
		builder.headers(name, value);
	}

	void HTTPResponse::setBody(const char* body)
	{
		this->body = body;
	}


	interfaces::IHTTPResponse* HTTPResponse::appendBody(const char* body)
	{
		this->body += body;

		return this;
	}

	void HTTPResponse::addCookie(const char* name, const char* value)
	{
		builder.headers
		(
			"Set-Cookie", format("{}={}", name, value)
		);
	}

	void HTTPResponse::setDefault()
	{
		builder.clear();

		builder.responseCode(web::ResponseCodes::ok);

		body.clear();

		this->setIsValid(true);
	}

	void HTTPResponse::setIsValid(bool isValid)
	{
		this->isValid = isValid;
	}

	HTTPResponse::operator bool() const
	{
		return isValid;
	}

	streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HTTPResponse& response)
	{
		string result;

		response.builder.headers
		(
			"Date", HTTPResponse::getFullDate(),
			"Server", "WebFramework-Server"
		);

		if (response.body.size())
		{
			result = response.builder.build(response.body);
		}
		else
		{
			result = response.builder.build();
		}

		stream << result;

		return stream;
	}
}
