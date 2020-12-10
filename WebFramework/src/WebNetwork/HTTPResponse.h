#pragma once

#include "HTTPBuilder.h"
#include "BaseIOSocketStream.h"

namespace framework
{
	class HTTPResponse
	{
	private:
		web::HTTPBuilder builder;
		std::string body;

	private:
		static std::string getFullDate();

	public:
		HTTPResponse();

		HTTPResponse& operator = (const web::HTTPBuilder& builder);

		void setHTTPVersion(const std::string& version);

		void setResponseCode(web::ResponseCodes code);

		void addHeader(const std::string& name, const std::string& value);

		void addBody(const std::string& body);

		void setDefault();

		friend streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HTTPResponse& response);

		~HTTPResponse() = default;
	};
}