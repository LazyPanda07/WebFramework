#include "HTTPResponseImplementation.h"

#include <chrono>

using namespace std;

namespace framework
{
	string HTTPResponseImplementation::getFullDate()
	{
		return format("{:%a, %d %b %Y %H:%M:%OS} GMT", chrono::system_clock::now());
	}

	HTTPResponseImplementation::HTTPResponseImplementation()
	{
		this->setDefault();
	}

	HTTPResponseImplementation& HTTPResponseImplementation::operator = (const web::HTTPBuilder& builder)
	{
		this->builder = builder;

		return *this;
	}

	void HTTPResponseImplementation::setHTTPVersion(const char* version)
	{
		builder.HTTPVersion(version);
	}

	void HTTPResponseImplementation::setResponseCode(int64_t code)
	{
		builder.responseCode(static_cast<web::ResponseCodes>(code));
	}

	void HTTPResponseImplementation::addHeader(const char* name, const char* value)
	{
		builder.headers(name, value);
	}

	void HTTPResponseImplementation::setBody(const char* body)
	{
		this->body = body;
	}

	interfaces::IHTTPResponse* HTTPResponseImplementation::appendBody(const char* body)
	{
		this->body += body;

		return this;
	}

	void HTTPResponseImplementation::addCookie(const char* name, const char* value)
	{
		builder.headers
		(
			"Set-Cookie", format("{}={}", name, value)
		);
	}

	void HTTPResponseImplementation::setDefault()
	{
		builder.clear();

		builder.responseCode(web::ResponseCodes::ok);

		body.clear();

		this->setIsValid(true);
	}

	void HTTPResponseImplementation::setIsValid(bool isValid)
	{
		this->isValid = isValid;
	}

	bool HTTPResponseImplementation::getIsValid() const
	{
		return isValid;
	}

	HTTPResponseImplementation::operator bool() const
	{
		return this->getIsValid();
	}

	streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HTTPResponseImplementation& response)
	{
		string result;

		response.builder.headers
		(
			"Date", HTTPResponseImplementation::getFullDate(),
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
