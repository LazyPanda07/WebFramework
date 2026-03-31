#pragma once

#include "WebInterfaces/IHttpResponse.h"

#include <HttpBuilder.h>
#include <IOSocketStream.h>

#include "Framework/WebFrameworkPlatform.h"

namespace framework
{
	/// <summary>
	/// HTTPBuilder wrapper
	/// <para>Overriding output stream operator for simplify HTTP response sending</para>
	/// </summary>
	class WEB_FRAMEWORK_API HttpResponseImplementation : public interfaces::IHttpResponse
	{
	private:
		web::HttpBuilder builder;
		std::string body;
		bool isValid;
		bool defaultResponseCode;

	public:
		/// <summary>
		/// Adds full date GMT to response
		/// </summary>
		/// <returns>date in following format 'day,' 'day of the month' 'name of the month' 'full year' 'hours:minutes:seconds' GMT</returns>
		static std::string getFullDate();

	public:
		HttpResponseImplementation();

		HttpResponseImplementation& operator = (const web::HttpBuilder& builder);

		void setHTTPVersion(const char* version) override;

		void setResponseCode(int64_t code) override;

		void addHeader(const char* name, const char* value) override;

		void setBody(const char* body, size_t bodySize) override;

		void setJsonBodyWithBuilder(void* implementation) override;

		void setJsonBodyWithObject(void* implementation) override;

		interfaces::IHttpResponse* appendBody(const char* body) override;

		void addCookie(const char* name, const char* value) override;

		void setDefault() override;

		void setIsValid(bool isValid) override;

		bool getIsValid() const override;

		explicit operator bool() const;

		friend streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HttpResponseImplementation& response);

		~HttpResponseImplementation() = default;
	};
}
