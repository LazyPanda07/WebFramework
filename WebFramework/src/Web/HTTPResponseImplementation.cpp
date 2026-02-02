#include "HTTPResponseImplementation.h"

#include <chrono>
#include <ctime>

#include <Log.h>

namespace framework
{
	std::string HTTPResponseImplementation::getFullDate()
	{
		constexpr size_t bufferSize = 64;
		constexpr size_t resultSize = 96;

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t seconds = std::chrono::system_clock::to_time_t(now);
		std::tm time;

#ifdef __LINUX__
		gmtime_r(&seconds, &time);
#else
		gmtime_s(&time, &seconds);
#endif

		char buffer[bufferSize]{};
		char result[resultSize]{};

		if (std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S", &time) == 0)
		{
			Log::error("Can't put time into buffer", "LogTime");

			return "";
		}

		std::chrono::milliseconds milliseconds = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		std::snprintf(result, sizeof(result), "%s.%03d GMT", buffer, static_cast<int>(milliseconds.count()));

		return result;
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

		defaultResponseCode = false;
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

		defaultResponseCode = true;
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
