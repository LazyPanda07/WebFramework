#include "Web/HttpResponseImplementation.h"

#include <chrono>
#include <ctime>

#include <Log.h>

namespace framework
{
	std::string HttpResponseImplementation::getFullDate()
	{
		constexpr size_t resultSize = 64;

		std::time_t seconds = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::tm time;

#ifdef __LINUX__
		gmtime_r(&seconds, &time);
#else
		gmtime_s(&time, &seconds);
#endif

		char result[resultSize]{};

		if (std::strftime(result, sizeof(result), "%a, %d %b %Y %H:%M:%S %Z", &time) == 0)
		{
			Log::error("Can't put time into buffer", "LogTime");

			return "";
		}

		return result;
	}

	HttpResponseImplementation::HttpResponseImplementation()
	{
		this->setDefault();
	}

	HttpResponseImplementation& HttpResponseImplementation::operator = (const web::HttpBuilder& builder)
	{
		this->builder = builder;

		return *this;
	}

	void HttpResponseImplementation::setHTTPVersion(const char* version)
	{
		builder.HTTPVersion(version);
	}

	void HttpResponseImplementation::setResponseCode(int64_t code)
	{
		builder.responseCode(static_cast<web::ResponseCodes>(code));

		defaultResponseCode = false;
	}

	void HttpResponseImplementation::addHeader(const char* name, const char* value)
	{
		builder.headers(name, value);
	}

	void HttpResponseImplementation::setBody(const char* body, size_t bodySize)
	{
		this->body = std::string(body, bodySize);
	}

	interfaces::IHttpResponse* HttpResponseImplementation::appendBody(const char* body)
	{
		this->body += body;

		return this;
	}

	void HttpResponseImplementation::addCookie(const char* name, const char* value)
	{
		builder.headers
		(
			"Set-Cookie", std::format("{}={}", name, value)
		);
	}

	void HttpResponseImplementation::setDefault()
	{
		builder.clear();

		builder.responseCode(web::ResponseCodes::ok);

		body.clear();

		this->setIsValid(true);

		defaultResponseCode = true;
	}

	void HttpResponseImplementation::setIsValid(bool isValid)
	{
		this->isValid = isValid;
	}

	bool HttpResponseImplementation::getIsValid() const
	{
		return isValid;
	}

	HttpResponseImplementation::operator bool() const
	{
		return this->getIsValid();
	}

	streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HttpResponseImplementation& response)
	{
		std::string result;

		response.builder.headers
		(
			"Date", HttpResponseImplementation::getFullDate(),
			"Server", "WebFramework-Server"
		);

		if (response.body.size())
		{
			result = response.builder.build(response.body);
		}
		else
		{
			if (response.defaultResponseCode)
			{
				response.builder.responseCode(web::ResponseCodes::noContent);
			}

			result = response.builder.build();
		}

		stream << result;

		return stream;
	}
}
