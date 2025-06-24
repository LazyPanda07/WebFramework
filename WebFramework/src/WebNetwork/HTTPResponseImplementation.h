#pragma once

#include "HTTPBuilder.h"
#include "IOSocketStream.h"
#include "WebNetwork/Interfaces/IHTTPResponse.h"

#include "Framework/WebFrameworkPlatform.h"

namespace framework
{
	/// <summary>
	/// HTTPBuilder wrapper
	/// <para>Overriding output stream operator for simplify HTTP response sending</para>
	/// </summary>
	class WEB_FRAMEWORK_API HTTPResponseImplementation : public interfaces::IHTTPResponse
	{
	private:
		web::HTTPBuilder builder;
		std::string body;
		bool isValid;

	public:
		/// <summary>
		/// Adds full date GMT to response
		/// </summary>
		/// <returns>date in following format 'day,' 'day of the month' 'name of the month' 'full year' 'hours:minutes:seconds' GMT</returns>
		static std::string getFullDate();

	public:
		HTTPResponseImplementation();

		/// <summary>
		/// Assign operator for HTTPBuilder
		/// </summary>
		/// <param name="builder">your HTTPBuilder</param>
		/// <returns>self for builder pattern</returns>
		HTTPResponseImplementation& operator = (const web::HTTPBuilder& builder);

		/// <summary>
		/// <para>Set HTTP version</para>
		/// <para>Default is HTTP/1.1</para>
		/// </summary>
		/// <param name="version">full HTTP version like HTTP/1.1</param>
		void setHTTPVersion(const char* version) override;

		/// <summary>
		/// Set HTTP response code
		/// </summary>
		/// <param name="code">value from web::responseCodes</param>
		void setResponseCode(int code) override;

		/// <summary>
		/// Set additional HTTP header
		/// </summary>
		/// <param name="name">of HTTP header</param>
		/// <param name="value">of HTTP header</param>
		void addHeader(const char* name, const char* value) override;

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		void setBody(const char* body) override;

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		/// <returns>reference to self</returns>
		interfaces::IHTTPResponse* appendBody(const char* body) override;

		/// <summary>
		/// Add cookie to HTTP response
		/// </summary>
		/// <param name="name">of cookie</param>
		/// <param name="value">of cookie</param>
		void addCookie(const char* name, const char* value) override;

		/// <summary>
		/// Clears HTTPResponse
		/// </summary>
		void setDefault() override;

		void setIsValid(bool isValid) override;

		bool getIsValid() const override;

		/**
		* isValid getter
		*/
		explicit operator bool() const;

		/// <summary>
		/// Sending HTTPResponse through network
		/// </summary>
		/// <param name="stream">special class for sending data through network</param>
		/// <param name="response">class instance</param>
		/// <returns>reference to self</returns>
		/// <exception cref="web::WebException"></exception>
		friend streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HTTPResponseImplementation& response);

		~HTTPResponseImplementation() = default;
	};
}
