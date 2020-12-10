#include "HTTPResponse.h"

#include <ctime>

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	string HTTPResponse::getFullDate()
	{
		time_t epochTime = time(nullptr);
		tm calendarTime;
		string result;

		result.resize(64);

		gmtime_s(&calendarTime, &epochTime);

		result.resize(strftime(result.data(), result.size(), "%a, %d %b %Y %H:%M:%S GMT", &calendarTime));

		return result;
	}

	HTTPResponse::HTTPResponse()
	{
		builder.responseCode(web::ResponseCodes::ok);
	}

	HTTPResponse& HTTPResponse::operator = (const web::HTTPBuilder& builder)
	{
		this->builder = builder;

		return *this;
	}

	void HTTPResponse::setHTTPVersion(const string& version)
	{
		builder.HTTPVersion(version);
	}

	void HTTPResponse::setResponseCode(web::ResponseCodes code)
	{
		builder.responseCode(code);
	}

	void HTTPResponse::addHeader(const string& name, const string& value)
	{
		builder.headers
		(
			name, value
		);
	}

	void HTTPResponse::addBody(const string& body)
	{
		this->body = body;
	}

	void HTTPResponse::setDefault()
	{
		builder.clear();

		builder.responseCode(web::ResponseCodes::ok);

		body.clear();
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
			result = response.builder.build(&response.body);
		}
		else
		{
			result = response.builder.build();
		}

		stream << result;

		return stream;
	}
}