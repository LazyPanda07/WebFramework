#include "HTTPResponseImplementation.h"

#include <chrono>

namespace framework
{
	std::string HTTPResponseImplementation::getFullDate()
	{
		return std::format("{:%a, %d %b %Y %H:%M:%OS} GMT", std::chrono::system_clock::now());
	}

	HTTPResponseImplementation::HTTPResponseImplementation()
	{
		this->setDefault();
	}

	HTTPResponseImplementation& HTTPResponseImplementation::operator = (const web::HttpBuilder& builder)
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
			"Set-Cookie", std::format("{}={}", name, value)
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
		std::string result;

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
